/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(SOC_SWALLOW) && defined(HAL_CRU_MODULE_ENABLED)

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

/********************* Private Structure Definition **************************/
static struct PLL_CONFIG PLL_TABLE[] = {
    /* _mhz, _refDiv, _fbDiv, _postdDv1, _postDiv2, _dsmpd, _frac */
    RK_PLL_RATE(297000000, 2, 99, 4, 1, 1, 0),
    RK_PLL_RATE(250000000, 2, 125, 6, 1, 1, 0),
    { /* sentinel */ },
};

static uint32_t s_gpllFreq;
static uint32_t s_usbphyFreq = 480000000;

static struct PLL_SETUP GPLL = {
    .conOffset0 = &(CRU->GPLL_CON[0]),
    .conOffset1 = &(CRU->GPLL_CON[1]),
    .conOffset2 = &(CRU->GPLL_CON[2]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 0,
    .lockShift = 10,
    .modeMask = 0x3 << 0,
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
    uint32_t muxSrc = 0, mux = CLK_GET_MUX(clockName);
    uint32_t divSrc = 0, divFrac;
    uint32_t n, m, pRate;

    switch (clockName) {
    case CLK_UART0:
        muxSrc = CLK_GET_MUX(CLK_UART0_SRC);
        divSrc = CLK_GET_DIV(CLK_UART0_SRC);
        divFrac = CLK_GET_DIV(CLK_UART0_FRAC);
        break;
    case CLK_32K:
        divFrac = CLK_GET_DIV(CLK_32K);
        break;
    default:

        return HAL_INVAL;
    }

    n = (CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0xffff0000) >> 16;
    m = CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0x0000ffff;

    switch (clockName) {
    case CLK_UART0:
        if (HAL_CRU_ClkGetMux(muxSrc)) {
            pRate = PLL_INPUT_OSC_RATE;
        } else {
            pRate = s_gpllFreq;
        }
        break;
    case CLK_32K:
        pRate = PLL_INPUT_OSC_RATE;
        freq = (pRate / m) * n;

        return freq;
    default:

        return HAL_INVAL;
    }

    pRate = pRate / HAL_CRU_ClkGetDiv(divSrc);
    if (HAL_CRU_ClkGetMux(mux) == 0) {
        freq = pRate;
    } else {
        freq = (pRate / m) * n;
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
        muxSrc = CLK_GET_MUX(CLK_UART0_SRC);
        divSrc = CLK_GET_DIV(CLK_UART0_SRC);
        divFrac = CLK_GET_DIV(CLK_UART0_FRAC);
        gateId = CLK_UART_PLL_GATE;
        fracGateId = CLK_UART_FRAC_GATE;
        break;
    case CLK_32K:
        divFrac = CLK_GET_DIV(CLK_32K);
        break;
    default:

        return HAL_INVAL;
    }

    switch (clockName) {
    case CLK_UART0:
        HAL_CRU_ClkEnable(gateId);
        HAL_CRU_ClkEnable(fracGateId);
        if ((!(PLL_INPUT_OSC_RATE % rate)) && ((PLL_INPUT_OSC_RATE / rate) < 31)) {
            HAL_CRU_ClkSetDiv(divSrc, PLL_INPUT_OSC_RATE / rate);
            HAL_CRU_ClkSetMux(muxSrc, 1);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else if ((!(s_gpllFreq % rate)) && ((s_gpllFreq / rate) < 31)) {
            HAL_CRU_ClkSetDiv(divSrc, s_gpllFreq / rate);
            HAL_CRU_ClkSetMux(muxSrc, 0);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else {
            HAL_CRU_FracdivGetConfig(rate, s_gpllFreq, &n, &m);
            HAL_CRU_ClkSetDiv(divSrc, 1);
            HAL_CRU_ClkSetMux(muxSrc, 0);
            CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;
            HAL_CRU_ClkSetMux(mux, 1);
        }

        return HAL_OK;
    case CLK_32K:
        HAL_CRU_FracdivGetConfig(rate, PLL_INPUT_OSC_RATE, &n, &m);
        CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;

        return HAL_OK;
    default:

        return HAL_INVAL;
    }
}

/**
 * @brief Get clk freq.
 * @param  clockName: CLOCK_Name id.
 * @return rate.
 * @attention these APIs allow direct use in the HAL layer.
 */
HAL_SECTION_SRAM_CODE
uint32_t HAL_CRU_ClkGetFreq(eCLOCK_Name clockName)
{
    uint32_t freq;
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t pRate = s_gpllFreq;

    if (!s_gpllFreq) {
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    }
    switch (clockName) {
    case PLL_GPLL:
        freq = HAL_CRU_GetPllFreq(&GPLL);
        s_gpllFreq = freq;

        return freq;
    case PLL_GPLL_NP5:
        freq = (pRate * 2) / ((HAL_CRU_ClkGetDiv(clkDiv) - 1) * 2 + 3);

        return freq;
    case HCLK_LOGIC_BUS:
        pRate = HAL_CRU_ClkGetFreq(HCLK_MCU_BUS);
        break;
    case CLK_SARADC:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case CLK_UART0:
    case CLK_32K:
        freq = HAL_CRU_ClkFracGetFreq(clockName);

        return freq;
    case CLK_WDT:

        return PLL_INPUT_OSC_RATE;
    case HCLK_MCU_BUS:
        if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            pRate = s_usbphyFreq;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            pRate = HAL_CRU_ClkGetFreq(PLL_GPLL_NP5);
        }
        break;
    case ACLK_JPEG_BUS:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = HAL_CRU_ClkGetFreq(PLL_GPLL_NP5);
        }
        break;
    case CLK_SPI0:
    case CLK_SPI1:
    case CLK_PWM:
    case CLK_TIMER:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = PLL_INPUT_OSC_RATE;
        }
        break;
    case CLK_I2C0:
    case CLK_I2C1:
        if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            pRate = PLL_INPUT_OSC_RATE;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            pRate = HAL_CRU_ClkGetFreq(PLL_GPLL_NP5);
        }
        break;
    case CLK_XIP_SFC:
    case CLK_VIP_OUT:
        if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            pRate = PLL_INPUT_OSC_RATE;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            pRate = s_usbphyFreq;
        }
        break;
    case CLK_ISP:
        if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            return s_usbphyFreq / 2;
        } else {
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
    uint32_t clkDiv = CLK_GET_DIV(clockName), div = 0;
    uint32_t pRate = s_gpllFreq;

    if (!s_gpllFreq) {
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    }

    switch (clockName) {
    case PLL_GPLL:
        error = HAL_CRU_SetPllFreq(&GPLL, rate);
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);

        return error;
    case PLL_GPLL_NP5:
        div = (HAL_DIV_ROUND_UP(pRate * 2, rate) - 3) / 2;
        HAL_CRU_ClkSetDiv(clkDiv, div + 1);

        return error;
    case HCLK_LOGIC_BUS:
        pRate = HAL_CRU_ClkGetFreq(HCLK_MCU_BUS);
        break;
    case CLK_SARADC:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case CLK_UART0:
    case CLK_32K:
        error = HAL_CRU_ClkFracSetFreq(clockName, rate);

        return error;
    case CLK_WDT:

        return HAL_OK;
    case HCLK_MCU_BUS:
        if (!(s_usbphyFreq % rate)) {
            pRate = s_usbphyFreq;
            mux = HCLK_MCU_BUS_SEL_CLK_USBPLL_MUX;
        } else if (!(HAL_CRU_ClkGetFreq(PLL_GPLL_NP5) % rate)) {
            pRate = HAL_CRU_ClkGetFreq(PLL_GPLL_NP5);
            mux = HCLK_MCU_BUS_SEL_CLK_GPLL_MUX_NP5;
        } else {
            pRate = s_gpllFreq;
            mux = HCLK_MCU_BUS_SEL_CLK_GPLL_MUX;
        }
        break;
    case ACLK_JPEG_BUS:
        if (!(HAL_CRU_ClkGetFreq(PLL_GPLL_NP5) % rate)) {
            pRate = HAL_CRU_ClkGetFreq(PLL_GPLL_NP5);
            mux = ACLK_JPEG_BUS_SEL_CLK_GPLL_MUX_NP5;
        } else {
            pRate = s_gpllFreq;
            mux = ACLK_JPEG_BUS_SEL_CLK_GPLL_MUX;
        }
        break;
    case CLK_SPI0:
    case CLK_SPI1:
    case CLK_PWM:
    case CLK_TIMER:
        if (!(PLL_INPUT_OSC_RATE % rate)) {
            pRate = PLL_INPUT_OSC_RATE;
            mux = CLK_SPI0_SEL_XIN_OSC0_FUNC;
        } else {
            pRate = s_gpllFreq;
            mux = CLK_SPI0_SEL_CLK_GPLL_MUX;
        }
        break;
    case CLK_I2C0:
    case CLK_I2C1:
        if (!(PLL_INPUT_OSC_RATE % rate)) {
            pRate = PLL_INPUT_OSC_RATE;
            mux = CLK_I2C0_SEL_XIN_OSC0_FUNC;
        } else if (!(HAL_CRU_ClkGetFreq(PLL_GPLL_NP5) % rate)) {
            pRate = HAL_CRU_ClkGetFreq(PLL_GPLL_NP5);
            mux = CLK_I2C0_SEL_CLK_GPLL_MUX_NP5;
        } else {
            pRate = s_gpllFreq;
            mux = CLK_I2C0_SEL_CLK_GPLL_MUX;
        }
        break;
    case CLK_XIP_SFC:
    case CLK_VIP_OUT:
        if (!(PLL_INPUT_OSC_RATE % rate)) {
            pRate = PLL_INPUT_OSC_RATE;
            mux = CLK_XIP_SFC_SEL_XIN_OSC0_FUNC;
        } else if (!(s_usbphyFreq % rate)) {
            pRate = s_usbphyFreq;
            mux = CLK_VIP_OUT_SEL_CLK_USBPLL_MUX;
        } else {
            pRate = s_gpllFreq;
            mux = CLK_XIP_SFC_SEL_CLK_GPLL_MUX;
        }
        break;
    case CLK_ISP:
        if (rate == s_usbphyFreq / 2) {
            pRate = s_usbphyFreq / 2;
            mux = CLK_ISP_SEL_CLK_USBPLL_MUX_DIV2;

            return HAL_CRU_ClkSetMux(clkMux, mux);
        } else {
            pRate = s_gpllFreq;
            mux = CLK_ISP_SEL_CLK_ISP_DIV_OUT;
        }
        break;
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
        mask = CRU_GLB_RST_CON_WDT_GLB_SRST_EN_MASK | CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_MASK;
        val = (1 << CRU_GLB_RST_CON_WDT_GLB_SRST_EN_SHIFT) | (1 << CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT);
        break;
    case GLB_RST_SND_WDT0:
        mask = CRU_GLB_RST_CON_WDT_GLB_SRST_EN_MASK | CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_MASK;
        val = (1 << CRU_GLB_RST_CON_WDT_GLB_SRST_EN_SHIFT);
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

#endif /* SOC_SWALLOW && HAL_CRU_MODULE_ENABLED */
