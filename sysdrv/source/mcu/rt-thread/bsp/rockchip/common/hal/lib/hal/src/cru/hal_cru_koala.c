/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(RKMCU_KOALA) && defined(HAL_CRU_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CRU
 *  @{
 */

/** @defgroup CRU_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#define HCLK_MCU_MAX_RATE (104 * MHZ)
#define PCLK_MCU_MAX_RATE (26 * MHZ)
#define OSC_32KHZ         32768
#define RFPLL_RATE        (104 * MHZ)

/********************* Private Structure Definition **************************/

static struct PLL_CONFIG PLL_TABLE[] = {
    /* _mhz, _refDiv, _fbDiv, _postdDv1, _postDiv2, _dsmpd, _frac */
    RK_PLL_RATE(240000000, 13, 960, 4, 2, 1, 0),
    RK_PLL_RATE(104000000, 1, 96, 6, 4, 1, 0),
    { /* sentinel */ },
};

static uint32_t s_cpllFreq;
static uint32_t s_gpllFreq;

static struct PLL_SETUP CPLL = {
    .conOffset0 = &(CRU->CPLL_CON[0]),
    .conOffset1 = &(CRU->CPLL_CON[1]),
    .conOffset2 = &(CRU->CPLL_CON[2]),
    .conOffset3 = &(CRU->CPLL_CON[3]),
    .modeOffset = &(CRU->MODE_CON),
    .stat0 = &(CRU->CPLL_STAT0),
    .modeShift = 0,
    .lockShift = 0,
    .modeMask = 0x3 << 0,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP GPLL = {
    .conOffset0 = &(CRU->GPLL_CON[0]),
    .conOffset1 = &(CRU->GPLL_CON[1]),
    .conOffset2 = &(CRU->GPLL_CON[2]),
    .conOffset3 = &(CRU->GPLL_CON[3]),
    .modeOffset = &(CRU->MODE_CON),
    .stat0 = &(CRU->GPLL_STAT0),
    .modeShift = 2,
    .lockShift = 0,
    .modeMask = 0x3 << 2,
    .rateTable = PLL_TABLE,
};

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup CRU_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief Get frac clk freq.
 * @param  clockName: CLOCK_Name id
 * @return clk rate.
 * How to calculate the Frac clk divider:
 *     numerator is frac register[31:16]
 *     denominator is frac register[15:0]
 *     clk rate = pRate * numerator / denominator
 *     for a better jitter, pRate > 20 * rate
 */
static uint32_t HAL_CRU_ClkFracGetFreq(eCLOCK_Name clockName)
{
    uint32_t freq = 0;
    uint32_t muxSrc, mux = CLK_GET_MUX(clockName);
    uint32_t divSrc, divFrac;
    uint32_t n, m, pRate;

    switch (clockName) {
    case CLK_UART0:
        muxSrc = CLK_GET_MUX(CLK_UART0_SRC);
        divSrc = CLK_GET_DIV(CLK_UART0_SRC);
        divFrac = CLK_GET_DIV(CLK_UART0_FRAC);
        break;
    case CLK_UART1:
        muxSrc = CLK_GET_MUX(CLK_UART1_SRC);
        divSrc = CLK_GET_DIV(CLK_UART1_SRC);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        break;
    case CLK_I2S_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S_TX_FRAC);
        break;
    case CLK_I2S_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S_RX_FRAC);
        break;
    case CLK_PDM:
        muxSrc = CLK_GET_MUX(CLK_PDM_SRC);
        divSrc = CLK_GET_DIV(CLK_PDM_SRC);
        divFrac = CLK_GET_DIV(CLK_PDM_FRAC);
        break;
    default:

        return HAL_INVAL;
    }

    n = (CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0xffff0000) >> 16;
    m = CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0x0000ffff;

    if (HAL_CRU_ClkGetMux(muxSrc)) {
        pRate = s_gpllFreq / HAL_CRU_ClkGetDiv(divSrc);
    } else {
        pRate = s_cpllFreq / HAL_CRU_ClkGetDiv(divSrc);
    }

    if (HAL_CRU_ClkGetMux(mux) == UART0_CLK_SEL_INTEGRATION) {
        freq = pRate;
    } else if (HAL_CRU_ClkGetMux(mux) == UART0_CLK_SEL_FRACTION) {
        freq = (pRate / m) * n;
    } else if (HAL_CRU_ClkGetMux(mux) == UART0_CLK_SEL_XIN_OSC) {
        freq = PLL_INPUT_OSC_RATE;
    }

    return freq;
}

/**
 * @brief Set frac clk freq.
 * @param  clockName: CLOCK_Name id.
 * @param  rate: clk set rate
 * @return HAL_Status.
 * How to calculate the Frac clk divider:
 *     if pRate > 20 * rate, select frac divider
 *     else select normal divider, but the clk rate may be not accurate
 */
static HAL_Status HAL_CRU_ClkFracSetFreq(eCLOCK_Name clockName, uint32_t rate)
{
    uint32_t muxSrc, mux = CLK_GET_MUX(clockName);
    uint32_t divSrc, divFrac;
    uint32_t n = 0, m = 0;
    uint32_t gateId, fracGateId;

    switch (clockName) {
    case CLK_UART0:
        if (rate == PLL_INPUT_OSC_RATE) {
            HAL_CRU_ClkSetMux(mux, UART0_CLK_SEL_XIN_OSC);
            HAL_CRU_ClkDisable(CLK_UART0_SRC_GATE);

            return HAL_OK;
        }
        muxSrc = CLK_GET_MUX(CLK_UART0_SRC);
        divSrc = CLK_GET_DIV(CLK_UART0_SRC);
        divFrac = CLK_GET_DIV(CLK_UART0_FRAC);
        gateId = CLK_UART0_SRC_GATE;
        fracGateId = CLK_UART0_FRAC_SRC_GATE;
        break;
    case CLK_UART1:
        if (rate == PLL_INPUT_OSC_RATE) {
            HAL_CRU_ClkSetMux(mux, UART1_CLK_SEL_XIN_OSC);
            HAL_CRU_ClkDisable(CLK_UART1_SRC_GATE);

            return HAL_OK;
        }
        muxSrc = CLK_GET_MUX(CLK_UART1_SRC);
        divSrc = CLK_GET_DIV(CLK_UART1_SRC);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        gateId = CLK_UART1_SRC_GATE;
        fracGateId = CLK_UART1_FRAC_SRC_GATE;
        break;
    case CLK_I2S_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S_TX_FRAC);
        gateId = CLK_I2S_TX_SRC_GATE;
        fracGateId = CLK_I2S_TX_FRAC_SRC_GATE;
        break;
    case CLK_I2S_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S_RX_FRAC);
        gateId = CLK_I2S_RX_SRC_GATE;
        fracGateId = CLK_I2S_RX_FRAC_SRC_GATE;
        break;
    case CLK_PDM:
        muxSrc = CLK_GET_MUX(CLK_PDM_SRC);
        divSrc = CLK_GET_DIV(CLK_PDM_SRC);
        divFrac = CLK_GET_DIV(CLK_PDM_FRAC);
        gateId = CLK_PDM_SRC_GATE;
        fracGateId = CLK_PDM_FRAC_SRC_GATE;
        break;
    default:

        return HAL_INVAL;
    }

    HAL_CRU_ClkEnable(gateId);
    HAL_CRU_ClkEnable(fracGateId);

    if ((!(s_cpllFreq % rate)) && ((s_cpllFreq / rate) < 16)) {
        HAL_CRU_ClkSetDiv(divSrc, s_cpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, UART0_CLK_SRC_SEL_CPLL);
        HAL_CRU_ClkSetMux(mux, UART0_CLK_SEL_INTEGRATION);
        HAL_CRU_ClkDisable(fracGateId);
    } else {
        HAL_CRU_FracdivGetConfig(rate, s_cpllFreq, &n, &m);
        HAL_CRU_ClkSetDiv(divSrc, 1);
        HAL_CRU_ClkSetMux(muxSrc, UART0_CLK_SRC_SEL_CPLL);
        HAL_CRU_ClkSetMux(mux, UART0_CLK_SEL_FRACTION);
        CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;
    }

    return HAL_OK;
}

/**
 * @brief Get clk freq.
 * @param  clockName: CLOCK_Name id.
 * @return rate.
 * @attention these APIs allow direct use in the HAL layer.
 */
uint32_t HAL_CRU_ClkGetFreq(eCLOCK_Name clockName)
{
    uint32_t clkMux = CLK_GET_MUX(clockName), mux = 0;
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t freq, pRate = 0;

    if (!s_cpllFreq) {
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);
    }
    if (!s_gpllFreq) {
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    }

    switch (clockName) {
    case PLL_CPLL:
        freq = HAL_CRU_GetPllFreq(&CPLL);
        s_cpllFreq = freq;

        return freq;
    case PLL_GPLL:
        freq = HAL_CRU_GetPllFreq(&GPLL);
        s_gpllFreq = freq;

        return freq;
    case HCLK_MCU:
        mux = HAL_CRU_ClkGetMux(clkMux);
        if (mux == HCLK_MCU_SRC_SEL_CPLL) {
            pRate = s_cpllFreq;
        } else {
            pRate = s_gpllFreq;
        }
        break;
    case PCLK_MCU:
    case STCLK_M3:
        pRate = HAL_CRU_ClkGetFreq(HCLK_MCU);
        break;
    case CLK_UART0:
    case CLK_UART1:
    case CLK_I2S_TX:
    case CLK_I2S_RX:
    case CLK_PDM:
        freq = HAL_CRU_ClkFracGetFreq(clockName);

        return freq;
    case CLK_TIMER0:
    case CLK_TIMER1:
    case CLK_TIMER2:
    case CLK_TIMER3:
    case CLK_TIMER4:
    case CLK_PVTM:
    case CLK_GPIO0:
    case CLK_GPIO1:
    case CLK_GPIO2:
        mux = HAL_CRU_ClkGetMux(clkMux);
        if (mux == CLK_TIMER0_SRC_SEL_32KHZ) {
            pRate = OSC_32KHZ;
        } else {
            pRate = PLL_INPUT_OSC_RATE;
        }
        break;
    case CLK_PWM:
    case CLK_SPI1:
    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_I2C2:
    case PCLK_PMU:
        pRate = s_gpllFreq;
        break;
    case CLK_BT52M:
        mux = HAL_CRU_ClkGetMux(clkMux);
        if (mux == CLK_BT52M_SRC_SEL_CPLL) {
            pRate = s_cpllFreq;
        } else if (mux == CLK_BT52M_SRC_SEL_GPLL) {
            pRate = s_gpllFreq;
        } else if (mux == CLK_BT52M_SRC_SEL_RFPLL) {
            pRate = RFPLL_RATE;
        }
        break;
    case HCLK_BT:
        mux = HAL_CRU_ClkGetMux(clkMux);
        if (mux == HCLK_BT_SRC_SEL_CPLL) {
            pRate = s_cpllFreq;
        } else if (mux == HCLK_BT_SRC_SEL_GPLL) {
            pRate = s_gpllFreq;
        }
        break;
    case CLK_SFC:
    case CLK_DSP:
        mux = HAL_CRU_ClkGetMux(clkMux);
        if (mux == CLK_SFC_SRC_SEL_CPLL) {
            pRate = s_cpllFreq;
        } else if (mux == CLK_SFC_SRC_SEL_GPLL) {
            pRate = s_gpllFreq;
        }
        break;
    case CLK_EFUSE:
    case CLK_BBGEN:
        mux = HAL_CRU_ClkGetMux(clkMux);
        if (mux == CLK_EFUSE_SEL_XIN_OSC) {
            pRate = PLL_INPUT_OSC_RATE;
        } else if (mux == CLK_EFUSE_SEL_GPLL) {
            pRate = s_gpllFreq;
        }
        break;
    default:
        break;
    }

    if ((clkMux == 0) && (clkDiv == 0)) {
        return 0;
    }

    if (clkDiv) {
        freq = pRate / (HAL_CRU_ClkGetDiv(clkDiv));
    } else {
        freq = pRate;
    }

    return freq;
}

/**
 * @brief Set clk freq.
 * @param  clockName: CLOCK_Name id.
 * @param  rate: clk rate.
 * @return HAL_Status.
 * @attention these APIs allow direct use in the HAL layer.
 */
HAL_Status HAL_CRU_ClkSetFreq(eCLOCK_Name clockName, uint32_t rate)
{
    HAL_Status error = HAL_OK;
    uint32_t clkMux = CLK_GET_MUX(clockName), mux = 0;
    uint32_t clkDiv = CLK_GET_DIV(clockName), div = 0, oldDiv = 0;
    uint32_t pRate = 0, tmpRate = 0;

    if (!s_cpllFreq) {
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);
    }
    if (!s_gpllFreq) {
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    }

    switch (clockName) {
    case PLL_CPLL:
        error = HAL_CRU_SetPllFreq(&CPLL, rate);
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);

        return error;
    case PLL_GPLL:
        error = HAL_CRU_SetPllFreq(&GPLL, rate);
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);

        return error;
    case HCLK_MCU:
        if (!(s_gpllFreq % rate)) {
            /*
             * CPLL=240MHz, GPLL=104MHz
             * change src from cpll to gpll(big to small), set mux first
             */
            HAL_CRU_ClkSetMux(clkMux, HCLK_MCU_SRC_SEL_GPLL);
            /* hclk_mcu=104MHz, pclk_mcu=26MHz */
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(PCLK_MCU), rate / PCLK_MCU_MAX_RATE);
            HAL_CRU_ClkSetDiv(clkDiv, s_gpllFreq / rate);
            /* stclk_m3 is equal to hclk_mcu */
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(STCLK_M3), 1);
        } else {
            /*
             * CPLL=240MHz, GPLL=104MHz
             * change src from gpll to cpll(small to big), set div first
             * hclk_mcu=80MHz/60MHz, pclk_mcu<=26MHz, set pclk_mcu to a small rate first
             */
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(PCLK_MCU), HCLK_MCU_MAX_RATE / PCLK_MCU_MAX_RATE);
            HAL_CRU_ClkSetDiv(clkDiv, s_cpllFreq / rate);
            /* stclk_m3 is equal to hclk_mcu */
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(STCLK_M3), 1);
            /* pclk_mcu<=26MHz */
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(PCLK_MCU), rate / PCLK_MCU_MAX_RATE);
            HAL_CRU_ClkSetMux(clkMux, HCLK_MCU_SRC_SEL_CPLL);
        }

        return HAL_OK;
    case PCLK_MCU:
    case STCLK_M3:
        pRate = HAL_CRU_ClkGetFreq(HCLK_MCU);
        break;
    case CLK_UART0:
    case CLK_UART1:
    case CLK_I2S_TX:
    case CLK_I2S_RX:
    case CLK_PDM:
        error = HAL_CRU_ClkFracSetFreq(clockName, rate);

        return error;
    case CLK_TIMER0:
    case CLK_TIMER1:
    case CLK_TIMER2:
    case CLK_TIMER3:
    case CLK_TIMER4:
    case CLK_PVTM:
    case CLK_GPIO0:
    case CLK_GPIO1:
    case CLK_GPIO2:
        if (rate == PLL_INPUT_OSC_RATE) {
            mux = CLK_TIMER0_SRC_SEL_XIN_OSC;
            pRate = PLL_INPUT_OSC_RATE;
        } else if (rate == OSC_32KHZ) {
            mux = CLK_TIMER0_SRC_SEL_32KHZ;
            pRate = OSC_32KHZ;
        } else {
            return HAL_INVAL;
        }
        break;
    case CLK_PWM:
    case CLK_SPI1:
    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_I2C2:
    case PCLK_PMU:
        pRate = s_gpllFreq;
        break;
    case CLK_BT52M:
        mux = CLK_BT52M_SRC_SEL_GPLL;
        pRate = s_gpllFreq;
        break;
    case HCLK_BT:
        mux = HCLK_BT_SRC_SEL_GPLL;
        pRate = s_gpllFreq;
        break;
    case CLK_SFC:
    case CLK_DSP:
        if (!(s_cpllFreq % rate)) {
            mux = CLK_SFC_SRC_SEL_CPLL;
            pRate = s_cpllFreq;
        } else {
            mux = CLK_SFC_SRC_SEL_GPLL;
            pRate = s_gpllFreq;
        }
        if (!rate) {
            return HAL_INVAL;
        }
        div = HAL_DIV_ROUND_UP(pRate, rate);
        oldDiv = HAL_CRU_ClkGetDiv(clkDiv);
        tmpRate = pRate / oldDiv;
        if (tmpRate > rate) {
            HAL_CRU_ClkSetDiv(clkDiv, div);
            HAL_CRU_ClkSetMux(clkMux, mux);
        } else {
            HAL_CRU_ClkSetMux(clkMux, mux);
            HAL_CRU_ClkSetDiv(clkDiv, div);
        }

        return HAL_OK;
    case CLK_EFUSE:
    case CLK_BBGEN:
        if (rate == PLL_INPUT_OSC_RATE) {
            mux = CLK_EFUSE_SEL_XIN_OSC;
            HAL_CRU_ClkSetMux(clkMux, mux);
        } else {
            mux = CLK_EFUSE_SEL_GPLL;
            div = HAL_DIV_ROUND_UP(s_gpllFreq, rate);
            HAL_CRU_ClkSetDiv(clkDiv, div);
            HAL_CRU_ClkSetMux(clkMux, mux);
        }

        return HAL_OK;
    default:
        break;
    }

    if ((clkMux == 0) && (clkDiv == 0)) {
        return HAL_INVAL;
    }

    div = HAL_DIV_ROUND_UP(pRate, rate);
    if (clkMux) {
        HAL_CRU_ClkSetMux(clkMux, mux);
    }
    if (clkDiv) {
        HAL_CRU_ClkSetDiv(clkDiv, div);
    }

    return HAL_OK;
}

/**
 * @brief wdt glbrst enable.
 * @param  wdtType: wdt reset type.
 * @return HAL_OK.
 * @attention these APIs allow direct use in the HAL layer.
 */
HAL_Status HAL_CRU_WdtGlbRstEnable(eCRU_WdtRstType wdtType)
{
    uint32_t mask = 0, val = 0;

    switch (wdtType) {
    case GLB_RST_FST_WDT0:
        mask = CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_MASK;
        val = (1 << CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT);
        break;
    case GLB_RST_SND_WDT0:
        mask = CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_MASK;
        val = 0;
        break;
    default:

        return HAL_INVAL;
    }

    CRU->GLB_RST_CON = VAL_MASK_WE(mask, val);

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* RKMCU_KOALA && HAL_CRU_MODULE_ENABLED */
