/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"

#ifdef HAL_ACDCDIG_MODULE_ENABLED
struct HAL_ACDCDIG_DEV g_acdcDigDev =
{
    .pReg = ACDCDIG,
    .hclk = PCLK_ACDC_DIG_GATE,
    .pd = PD_AUDIO,
};

static HAL_Status BSP_ACDCDIG_Init(void)
{
    return HAL_OK;
}

static HAL_Status BSP_ACDCDIG_DeInit(void)
{
    return HAL_OK;
}

#endif

#ifdef HAL_AUDIOPWM_MODULE_ENABLED
struct HAL_AUDIOPWM_DEV g_audioPwmDev =
{
    .pReg = AUDIOPWM,
    .mclk = CLK_AUDPWM,
    .mclkGate = CLK_AUDPWM_FRAC_GATE,
    .hclk = HCLK_AUDPWM_GATE,
    .txDmaData =
    {
        .addr = (uint32_t)&(AUDIOPWM->FIFO_ENTRY),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_AUDIOPWM,
        .dmac = DMA,
    },
};

static HAL_Status BSP_AUDIOPWM_Init(void)
{
    /* CLK Init */

    /* IO Init */
    return HAL_OK;
}

static HAL_Status BSP_AUDIOPWM_DeInit(void)
{
    /* CLK Init */

    /* IO Init */
    return HAL_OK;
}
#endif

#ifdef HAL_I2C_MODULE_ENABLED
const struct HAL_I2C_DEV g_i2c0Dev =
{
    .pReg = I2C0,
    .clkID = CLK_I2C0,
    .clkGateID = CLK_I2C0_GATE,
    .pclkGateID = PCLK_I2C0_GATE,
    .irqNum = I2CMST0_IRQn,
    .runtimeID = PM_RUNTIME_ID_I2C0,
};

const struct HAL_I2C_DEV g_i2c1Dev =
{
    .pReg = I2C1,
    .clkID = CLK_I2C1,
    .clkGateID = CLK_I2C1_GATE,
    .pclkGateID = PCLK_I2C1_GATE,
    .irqNum = I2CMST1_IRQn,
    .runtimeID = PM_RUNTIME_ID_I2C1,
};

const struct HAL_I2C_DEV g_i2c2Dev =
{
    .pReg = I2C2,
    .clkID = CLK_I2C2,
    .clkGateID = CLK_I2C2_GATE,
    .pclkGateID = PCLK_I2C2_GATE,
    .irqNum = I2CMST2_IRQn,
    .runtimeID = PM_RUNTIME_ID_I2C2,
};
#endif

#ifdef HAL_I2STDM_MODULE_ENABLED
struct HAL_I2STDM_DEV g_i2sTdm0Dev =
{
    .pReg = I2STDM0,
    .mclkTx = I2S_MCLKOUT,
    .mclkTxGate = CLK_I2S8CH_GATE,
    .mclkRx = I2S_MCLKOUT,
    .mclkRxGate = CLK_I2S8CH_GATE,
    .hclk = HCLK_I2S_8CH_GATE,
    .pd = PD_AUDIO,
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
    .mclkTx = I2S1_MCLKOUT,
    .mclkTxGate = CLK_I2S1_8CH_GATE,
    .mclkRx = I2S1_MCLKOUT,
    .mclkRxGate = CLK_I2S1_8CH_GATE,
    .hclk = HCLK_I2S1_8CH_GATE,
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

static HAL_Status BSP_I2STDM_Init(void)
{
    /* CLK Init */

    /* IO Init */
    return HAL_OK;
}

static HAL_Status BSP_I2STDM_DeInit(void)
{
    /* CLK Init */

    /* IO Init */
    return HAL_OK;
}
#endif

#ifdef HAL_PDM_MODULE_ENABLED
struct HAL_PDM_DEV g_pdm0Dev =
{
    .pReg = PDM0,
    .mclk = MCLK_PDM0,
    .mclkGate = MCLK_PDM0_GATE,
#ifdef IS_FPGA
    .mclkRate = PDM_FPGA_CLK_RATE,
#else
    .mclkRate = PDM_CLK_RATE,
#endif
    .hclk = HCLK_PDM0_GATE,
    .pd = PD_AUDIO,
    .reset = SRST_M_PDM0,
    .rxDmaData =
    {
        .addr = (uint32_t)&(PDM0->RXFIFO_DATA_REG),
        .addrWidth = DMA_SLAVE_BUSWIDTH_4_BYTES,
        .maxBurst = 8,
        .dmaReqCh = DMA_REQ_PDM0,
        .dmac = DMA,
    },
};

static HAL_Status BSP_PDM_Init(void)
{
    /* CLK Init */

    /* IO Init */
    return HAL_OK;
}

static HAL_Status BSP_PDM_DeInit(void)
{
    /* CLK Init */

    /* IO Init */
    return HAL_OK;
}
#endif

#ifdef HAL_PL330_MODULE_ENABLED
struct HAL_PL330_DEV g_pl330Dev =
{
    .pReg = DMA,
    .peripReqType = BURST,
    .irq[0] = DMAC_IRQn,
    .irq[1] = DMAC_ABORT_IRQn,
    .pd = PD_AUDIO,
};

static HAL_Status BSP_PL330_Init(void)
{
    switch (g_pl330Dev.peripReqType) {
    case SINGLE:
        /* config all dma reqs type as single req mode */
        GRF->DMAC_CON3 = 0x30000000;
        GRF->DMAC_CON5 = 0xffff0000;
        GRF->DMAC_CON6 = 0xffff0000;
        break;
    case BURST:
        /* config all dma reqs type as burst req mode */
        GRF->DMAC_CON3 = 0x30001000;
        GRF->DMAC_CON5 = 0xffff5555;
        GRF->DMAC_CON6 = 0xffff5555;
        break;
    default:

        return HAL_ERROR;
    }

    return HAL_OK;
}

static HAL_Status BSP_PL330_DeInit(void)
{
    return HAL_OK;
}
#endif

#ifdef HAL_PWM_MODULE_ENABLED
const struct HAL_PWM_DEV g_pwm0Dev =
{
    .pReg = PWM0,
    .clkID = CLK_PWM,
    .clkGateID = CLK_PWM_GATE,
    .pclkGateID = PCLK_PWM_GATE,
    .irqNum = PWM_IRQn,
};
#endif

#ifdef HAL_VAD_MODULE_ENABLED
/* VAD_CONTROL[3:1]: voice source mapping */
const struct AUDIO_SRC_ADDR_MAP g_audioSrcAddrMaps[] =
{
    { 1, I2STDM0_BASE + 0x800 },
    { 2, PDM0_BASE + 0x400 },
    { 0 /* sentinel */ }
};

struct HAL_VAD_DEV g_vadDev =
{
    .pReg = VAD,
    .hclk = HCLK_VAD_GATE,
    .pd = PD_AUDIO,
    .irq = VAD_IRQn,
};

static HAL_Status BSP_VAD_Init(void)
{
    /* CLK Init */

    return HAL_OK;
}

static HAL_Status BSP_VAD_DeInit(void)
{
    /* CLK Init */

    return HAL_OK;
}
#endif

#if defined(HAL_PINCTRL_MODULE_ENABLED)
static const struct PINCTRL_BANK_INFO pinBanks[] = {
    PIN_BANK_CFG_FLAGS(GPIO_BANK0, 32, GRF_BASE,
                       0x0000, 4, 4,
                       0x0080, 2, 8,
                       0x00c0, 2, 8,
                       0x0040, 1, 16,
                       0x0060, 1, 16),
    PIN_BANK_CFG_FLAGS(GPIO_BANK1, 32, GRF_BASE,
                       0x0020, 4, 4,
                       0x0090, 2, 8,
                       0x00d0, 2, 8,
                       0x0048, 1, 16,
                       0x0068, 1, 16),
};

static const struct PINCTRL_MUX_ROUTE_DATA muxRouteData[] = {
    {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C0 | GPIO_PIN_C1,
        .func = 1, .routeReg = 0x114, .routeVal = ROUTE_VAL(0, 0, 0x3), //i2cmst0
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D3 | GPIO_PIN_D4,
        .func = 2, .routeReg = 0x114, .routeVal = ROUTE_VAL(1, 0, 0x3),
    }, {
        .bank = GPIO_BANK1, .pin = GPIO_PIN_A2 | GPIO_PIN_A3,
        .func = 5, .routeReg = 0x114, .routeVal = ROUTE_VAL(2, 0, 0x3),
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C2 | GPIO_PIN_C3,
        .func = 2, .routeReg = 0x114, .routeVal = ROUTE_VAL(0, 2, 0x3), //i2cmst1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B2 | GPIO_PIN_B3,
        .func = 2, .routeReg = 0x114, .routeVal = ROUTE_VAL(1, 2, 0x3),
    }, {
        .bank = GPIO_BANK1, .pin = GPIO_PIN_C0 | GPIO_PIN_C1,
        .func = 2, .routeReg = 0x114, .routeVal = ROUTE_VAL(2, 2, 0x3),
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C4 | GPIO_PIN_C5,
        .func = 2, .routeReg = 0x114, .routeVal = ROUTE_VAL(0, 4, 0x3), //i2cmst2
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B4 | GPIO_PIN_B5,
        .func = 2, .routeReg = 0x114, .routeVal = ROUTE_VAL(1, 4, 0x3),
    }, {
        .bank = GPIO_BANK1, .pin = GPIO_PIN_C2 | GPIO_PIN_C3,
        .func = 2, .routeReg = 0x114, .routeVal = ROUTE_VAL(2, 4, 0x3),
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D3 | GPIO_PIN_D4 | GPIO_PIN_D5 | GPIO_PIN_D6,
        .func = 4, .routeReg = 0x114, .routeVal = ROUTE_VAL(0, 6, 0x1), //pcm
    }, {
        .bank = GPIO_BANK1, .pin = GPIO_PIN_A7 | GPIO_PIN_B0 | GPIO_PIN_B1 | GPIO_PIN_B2 | GPIO_PIN_B4,
        .func = 2, .routeReg = 0x114, .routeVal = ROUTE_VAL(1, 6, 0x1),
    }, {
        .bank = GPIO_BANK1, .pin = GPIO_PIN_B4,
        .func = 1, .routeReg = 0x114, .routeVal = ROUTE_VAL(1, 6, 0x1),
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D1 | GPIO_PIN_D2,
        .func = 2, .routeReg = 0x114, .routeVal = ROUTE_VAL(0, 7, 0x3), //uart1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_A4 /* | GPIO_PIN_A5*/,
        .func = 2, .routeReg = 0x114, .routeVal = ROUTE_VAL(1, 7, 0x3),
    }, {
        .bank = GPIO_BANK1, .pin = GPIO_PIN_B0 | GPIO_PIN_B1,
        .func = 3, .routeReg = 0x114, .routeVal = ROUTE_VAL(2, 7, 0x3),
    }, {
/*      .bank = GPIO_BANK0, .pin = GPIO_PIN_A5,
        .func = 2, .routeReg = 0x114, .routeVal = ROUTE_VAL(3, 7, 0x3),
    }, {*/
        .bank = GPIO_BANK0, .pin = GPIO_PIN_B1,
        .func = 4, .routeReg = 0x114, .routeVal = ROUTE_VAL(3, 7, 0x3),
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_C2 | GPIO_PIN_C3 | GPIO_PIN_C4 | GPIO_PIN_C5,
        .func = 1, .routeReg = 0x114, .routeVal = ROUTE_VAL(0, 9, 0x1), //spimst1
    }, {
        .bank = GPIO_BANK1, .pin = GPIO_PIN_C0 | GPIO_PIN_C1 | GPIO_PIN_C2 | GPIO_PIN_C3,
        .func = 1, .routeReg = 0x114, .routeVal = ROUTE_VAL(1, 9, 0x1),
    }, {
        .bank = GPIO_BANK1, .pin = GPIO_PIN_A0 | GPIO_PIN_A1 | GPIO_PIN_A2 | GPIO_PIN_A3,
        .func = 3, .routeReg = 0x114, .routeVal = ROUTE_VAL(0, 10, 0x1), //spimst2
    }, {
        .bank = GPIO_BANK1, .pin = GPIO_PIN_C4 | GPIO_PIN_C5 | GPIO_PIN_C6 | GPIO_PIN_C7,
        .func = 1, .routeReg = 0x114, .routeVal = ROUTE_VAL(1, 10, 0x1),
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_A1 | GPIO_PIN_A2 | GPIO_PIN_A4 | GPIO_PIN_A6 | GPIO_PIN_A7 | GPIO_PIN_B0,
        .func = 4, .routeReg = 0x114, .routeVal = ROUTE_VAL(0, 11, 0x1), //i2s1
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D3 | GPIO_PIN_D4,
        .func = 3, .routeReg = 0x114, .routeVal = ROUTE_VAL(1, 11, 0x1),
    }, {
        .bank = GPIO_BANK1, .pin = GPIO_PIN_A7,
        .func = 2, .routeReg = 0x114, .routeVal = ROUTE_VAL(0, 12, 0x1), //pcmclk
    }, {
        .bank = GPIO_BANK1, .pin = GPIO_PIN_B4,
        .func = 1, .routeReg = 0x114, .routeVal = ROUTE_VAL(1, 12, 0x1),
    }, {
        .bank = GPIO_BANK1, .pin = GPIO_PIN_B6 | GPIO_PIN_B7,
        .func = 1, .routeReg = 0x114, .routeVal = ROUTE_VAL(0, 13, 0x3), //key
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D1 | GPIO_PIN_D2,
        .func = 5, .routeReg = 0x114, .routeVal = ROUTE_VAL(1, 13, 0x3),
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D6,
        .func = 5, .routeReg = 0x114, .routeVal = ROUTE_VAL(2, 13, 0x3),
    }, {
        .bank = GPIO_BANK0, .pin = GPIO_PIN_D7,
        .func = 4, .routeReg = 0x114, .routeVal = ROUTE_VAL(2, 13, 0x3),
    },
};

const struct HAL_PINCTRL_DEV g_pinDev = {
    .banks = pinBanks,
    .banksNum = HAL_ARRAY_SIZE(pinBanks),
    .muxRouteData = muxRouteData,
    .muxRouteDataNum = HAL_ARRAY_SIZE(muxRouteData),
};
#endif

#ifdef HAL_UART_MODULE_ENABLED
const struct HAL_UART_DEV g_uart0Dev =
{
    .pReg = UART0,
    .sclkID = CLK_UART0,
    .sclkGateID = CLK_UART0_GATE,
    .pclkGateID = PCLK_UART0_GATE,
    .irqNum = UART0_IRQn,
    .isAutoFlow = true,
    .runtimeID = PM_RUNTIME_ID_UART0,
};

const struct HAL_UART_DEV g_uart1Dev =
{
    .pReg = UART1,
    .sclkID = CLK_UART1,
    .sclkGateID = CLK_UART1_GATE,
    .pclkGateID = PCLK_UART1_GATE,
    .irqNum = UART1_IRQn,
    .isAutoFlow = false,
    .runtimeID = PM_RUNTIME_ID_UART1,
};

const struct HAL_UART_DEV g_uart2Dev =
{
    .pReg = UART2,
    .sclkID = CLK_UART2,
    .sclkGateID = CLK_UART2_GATE,
    .pclkGateID = PCLK_UART2_GATE,
    .irqNum = UART2_IRQn,
    .isAutoFlow = true,
    .runtimeID = PM_RUNTIME_ID_UART2,
};
#endif

#ifdef HAL_PCD_MODULE_ENABLED
const struct HAL_USB_DEV g_usbdDev =
{
    .pReg = USB,
    .hclkGateID = HCLK_USB2CTRL_GATE,
    .utmiclkGateID = CLK_USB2CTRL_GATE,
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
    .sclkGate = SCLK_SFC_GATE,
    .hclkGate = HCLK_SFC_GATE,
    .xipClkGate = HCLK_XIP_SFC_GATE,
    .sclkID = SCLK_SFC_SRC,
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
    .sclkGate = SCLK_SFC1_GATE,
    .hclkGate = HCLK_SFC1_GATE,
    .xipClkGate = HCLK_XIP_SFC1_GATE,
    .sclkID = SCLK_SFC1_SRC,
    .irqNum = FSPI1_IRQn,
    .xipMemData = XIP_MAP1_BASE1,
    .xmmcDev[0] =
    {
        .type = DEV_PSRAM,
    },
};

static HAL_Status BSP_FSPI_Init(void)
{
    return HAL_OK;
}

static HAL_Status BSP_FSPI_DeInit(void)
{
    /* CLK Init */

    return HAL_OK;
}
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

const struct HAL_SPI_DEV g_spi2Dev = {
    .base = SPI2_BASE,
    .clkId = CLK_SPI2,
    .clkGateID = CLK_SPI2_GATE,
    .pclkGateID = PCLK_SPI2_GATE,
    .irqNum = SPIMST2_IRQn,
    .isSlave = false,
    .txDma = {
        .channel = DMA_REQ_SPI2_TX,
        .direction = DMA_MEM_TO_DEV,
        .addr = SPI2_BASE + 0x400,
        .dmac = DMA,
    },
    .rxDma = {
        .channel = DMA_REQ_SPI2_RX,
        .direction = DMA_DEV_TO_MEM,
        .addr = SPI2_BASE + 0x800,
        .dmac = DMA,
    },
};
#endif

__WEAK void BSP_MPU_Init(void)
{
}

void BSP_DeInit(void)
{
#ifdef HAL_ACDCDIG_MODULE_ENABLED
    BSP_ACDCDIG_DeInit();
#endif

#ifdef HAL_AUDIOPWM_MODULE_ENABLED
    BSP_AUDIOPWM_DeInit();
#endif

#ifdef HAL_I2STDM_MODULE_ENABLED
    BSP_I2STDM_DeInit();
#endif

#ifdef HAL_PDM_MODULE_ENABLED
    BSP_PDM_DeInit();
#endif

#ifdef HAL_VAD_MODULE_ENABLED
    BSP_VAD_DeInit();
#endif

#ifdef HAL_PL330_MODULE_ENABLED
    BSP_PL330_DeInit();
#endif

#ifdef HAL_FSPI_MODULE_ENABLED
    BSP_FSPI_DeInit();
#endif
}

void BSP_Init(void)
{
    BSP_MPU_Init();

#ifdef HAL_ACDCDIG_MODULE_ENABLED
    BSP_ACDCDIG_Init();
#endif

#ifdef HAL_PL330_MODULE_ENABLED
    BSP_PL330_Init();
#endif

#ifdef HAL_AUDIOPWM_MODULE_ENABLED
    BSP_AUDIOPWM_Init();
#endif

#ifdef HAL_I2STDM_MODULE_ENABLED
    BSP_I2STDM_Init();
#endif

#ifdef HAL_PDM_MODULE_ENABLED
    BSP_PDM_Init();
#endif

#ifdef HAL_VAD_MODULE_ENABLED
    BSP_VAD_Init();
#endif

#ifdef HAL_CRU_AS_FEATURE_ENABLED
    HAL_CRU_AsInit();
#endif

#ifdef HAL_FSPI_MODULE_ENABLED
    BSP_FSPI_Init();
#endif
}

void BSP_SetLoaderFlag(void)
{
    GRF->OS_REG0 = SYS_UPGRADE_FLAG;
}
