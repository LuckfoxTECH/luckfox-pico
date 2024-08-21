/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(SOC_RV1108) && defined(HAL_CRU_MODULE_ENABLED)

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
#define DCLK_LCDC_PLL_LIMIT_FREQ 800 * 1000000

/********************* Private Structure Definition **************************/

static struct PLL_CONFIG PLL_TABLE[] = {
    /* _mhz, _refDiv, _fbDiv, _postdDv1, _postDiv2, _dsmpd, _frac */
    RK_PLL_RATE(1200000000, 1, 50, 1, 1, 1, 0),
    RK_PLL_RATE(1188000000, 1, 99, 2, 1, 1, 0),
    RK_PLL_RATE(1008000000, 1, 84, 2, 1, 1, 0),
    RK_PLL_RATE(816000000, 1, 68, 2, 1, 1, 0),
    RK_PLL_RATE(600000000, 1, 75, 3, 1, 1, 0),
    { /* sentinel */ },
};

static uint32_t s_apllFreq;
static uint32_t s_dpllFreq;
static uint32_t s_gpllFreq;

static struct PLL_SETUP APLL = {
    .conOffset0 = &(CRU->APLL_CON[0]),
    .conOffset1 = &(CRU->APLL_CON[1]),
    .conOffset2 = &(CRU->APLL_CON[2]),
    .conOffset3 = &(CRU->APLL_CON[3]),
    .modeOffset = &(CRU->APLL_CON[3]),
    .modeShift = 8,
    .lockShift = 31,
    .modeMask = 0x1 << 8,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP DPLL = {
    .conOffset0 = &(CRU->DPLL_CON[0]),
    .conOffset1 = &(CRU->DPLL_CON[1]),
    .conOffset2 = &(CRU->DPLL_CON[2]),
    .conOffset3 = &(CRU->DPLL_CON[3]),
    .modeOffset = &(CRU->DPLL_CON[3]),
    .modeShift = 8,
    .lockShift = 31,
    .modeMask = 0x1 << 8,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP GPLL = {
    .conOffset0 = &(CRU->GPLL_CON[0]),
    .conOffset1 = &(CRU->GPLL_CON[1]),
    .conOffset2 = &(CRU->GPLL_CON[2]),
    .conOffset3 = &(CRU->GPLL_CON[3]),
    .modeOffset = &(CRU->GPLL_CON[3]),
    .modeShift = 8,
    .lockShift = 31,
    .modeMask = 0x1 << 8,
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
 * @attention these APIs allow direct use in the HAL layer.
 * How to calculate the Frac clk divider:
 *     numerator is frac register[31:16]
 *     denominator is frac register[15:0]
 *     clk rate = pRate * numerator / denominator
 *     for a better jitter, pRate > 20 * rate
 */
uint32_t HAL_CRU_ClkFracGetFreq(eCLOCK_Name clockName)
{
    uint32_t freq = 0;
    uint32_t mux = CLK_GET_MUX(clockName);
    uint32_t divSrc, divFrac;
    uint32_t n, m, pRate;

    switch (clockName) {
    case CLK_UART0:
        divSrc = CLK_GET_DIV(CLK_UART0_PLL);
        divFrac = CLK_GET_DIV(CLK_UART0_FRAC);
        break;
    case CLK_UART1:
        divSrc = CLK_GET_DIV(CLK_UART1_PLL);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        break;
    case CLK_UART2:
        divSrc = CLK_GET_DIV(CLK_UART2_PLL);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        break;
    case CLK_I2S0:
        divSrc = CLK_GET_DIV(CLK_I2S0_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S0_FRAC);
        break;
    case CLK_I2S1:
        divSrc = CLK_GET_DIV(CLK_I2S1_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S1_FRAC);
        break;
    case CLK_I2S2:
        divSrc = CLK_GET_DIV(CLK_I2S2_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S2_FRAC);
        break;
    default:

        return HAL_INVAL;
    }

    n = (CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0xffff0000) >> 16;
    m = CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0x0000ffff;

    pRate = s_gpllFreq / HAL_CRU_ClkGetDiv(divSrc);

    if (HAL_CRU_ClkGetMux(mux) == 0) {
        freq = pRate;
    } else if (HAL_CRU_ClkGetMux(mux) == 1) {
        freq = (pRate / m) * n;
    } else if (HAL_CRU_ClkGetMux(mux) == 3) {
        freq = PLL_INPUT_OSC_RATE / 2;
    }

    return freq;
}

/**
 * @brief Set frac clk freq.
 * @param  clockName: CLOCK_Name id.
 * @param  rate: clk set rate
 * @return HAL_Status.
 * @attention these APIs allow direct use in the HAL layer.
 * How to calculate the Frac clk divider:
 *     if pRate > 20 * rate, select frac divider
 *     else select normal divider, but the clk rate may be not accurate
 */
HAL_Status HAL_CRU_ClkFracSetFreq(eCLOCK_Name clockName, uint32_t rate)
{
    uint32_t muxSrc, mux = CLK_GET_MUX(clockName), muxOut = 0;
    uint32_t divSrc, divFrac;
    uint32_t n = 0, m = 0, pRate = s_gpllFreq / 2;

    switch (clockName) {
    case CLK_UART0:
        muxSrc = CLK_GET_MUX(CLK_UART0_PLL);
        divSrc = CLK_GET_DIV(CLK_UART0_PLL);
        divFrac = CLK_GET_DIV(CLK_UART0_FRAC);
        break;
    case CLK_UART1:
        muxSrc = CLK_GET_MUX(CLK_UART1_PLL);
        divSrc = CLK_GET_DIV(CLK_UART1_PLL);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        break;
    case CLK_UART2:
        muxSrc = CLK_GET_MUX(CLK_UART2_PLL);
        divSrc = CLK_GET_DIV(CLK_UART2_PLL);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        break;
    case CLK_I2S0:
        muxSrc = CLK_GET_MUX(CLK_I2S0_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S0_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S0_FRAC);
        muxOut = CLK_GET_MUX(CLK_I2S0_OUT);
        break;
    case CLK_I2S1:
        muxSrc = CLK_GET_MUX(CLK_I2S1_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S1_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S1_FRAC);
        break;
    case CLK_I2S2:
        muxSrc = CLK_GET_MUX(CLK_I2S2_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S2_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S2_FRAC);
        break;
    default:

        return HAL_INVAL;
    }

    if ((!(s_gpllFreq % rate)) && ((s_gpllFreq / rate) < 31)) {
        HAL_CRU_ClkSetDiv(divSrc, s_gpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 1);
        HAL_CRU_ClkSetMux(mux, 0);
    } else {
        HAL_CRU_FracdivGetConfig(rate, pRate, &n, &m);
        HAL_CRU_ClkSetDiv(divSrc, 2);
        HAL_CRU_ClkSetMux(muxSrc, 1);
        CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;
        HAL_CRU_ClkSetMux(mux, 1);
    }

    if (muxOut) {
        HAL_CRU_ClkSetMux(muxOut, 0);
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
    uint32_t freq;
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t pRate = s_gpllFreq;

    if (!s_apllFreq) {
        s_apllFreq = HAL_CRU_GetPllFreq(&APLL);
    }
    if (!s_gpllFreq) {
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    }
    if (!s_dpllFreq) {
        s_dpllFreq = HAL_CRU_GetPllFreq(&DPLL);
    }

    switch (clockName) {
    case PLL_GPLL:
        freq = HAL_CRU_GetPllFreq(&GPLL);
        s_gpllFreq = freq;

        return freq;
    case PLL_DPLL:
        freq = HAL_CRU_GetPllFreq(&DPLL);
        s_dpllFreq = freq;

        return freq;
    case PLL_APLL:
    case CLK_CPU:
        freq = HAL_CRU_GetPllFreq(&APLL);
        s_apllFreq = freq;

        return freq;
    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_I2S0:
    case CLK_I2S1:
    case CLK_I2S2:

        return HAL_CRU_ClkFracGetFreq(clockName);
    case PCLK_BUS:
    case HCLK_BUS:
        pRate = HAL_CRU_ClkGetFreq(ACLK_BUS);
        break;
    case PCLK_PERI:
    case HCLK_PERI:
        pRate = HAL_CRU_ClkGetFreq(ACLK_PERI);
        break;
    case CLK_TSADC:
    case CLK_SARADC:
    case CLK_CSI_OUT:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case HCLK_VIO:
    case PCLK_VIO:
        pRate = HAL_CRU_ClkGetFreq(ACLK_VIO0);
        break;
    case CLK_HDMI_CEC:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case CLK_CIF_OUT:
        if (HAL_CRU_ClkGetMux(CLK_GET_MUX(CLK_CIF_OUT_SRC))) {
            pRate = PLL_INPUT_OSC_RATE;
        }
        break;
    case CLK_DSP_PFM:
    case PCLK_DSP:
    case DSP_IOP:
    case DSP_EPP:
    case DSP_EDP:
    case DSP_EDAP:
        pRate = HAL_CRU_ClkGetFreq(CLK_DSP);
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
    uint32_t clkMux = CLK_GET_MUX(clockName), mux = 1;
    uint32_t clkDiv = CLK_GET_DIV(clockName), div = 0;
    uint32_t pRate = s_gpllFreq;

    if (!s_apllFreq) {
        s_apllFreq = HAL_CRU_GetPllFreq(&APLL);
    }
    if (!s_gpllFreq) {
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    }
    if (!s_dpllFreq) {
        s_dpllFreq = HAL_CRU_GetPllFreq(&DPLL);
    }

    switch (clockName) {
    case PLL_GPLL:
        error = HAL_CRU_SetPllFreq(&GPLL, rate);
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);

        return error;
    case PLL_DPLL:
        error = HAL_CRU_SetPllFreq(&DPLL, rate);
        s_dpllFreq = HAL_CRU_GetPllFreq(&DPLL);

        return error;
    case PLL_APLL:
    case CLK_CPU:
        if (rate > HAL_CRU_GetPllFreq(&APLL)) {
            error = HAL_CRU_ClkSetDiv(CLK_GET_DIV(PCLK_DBG), rate / 150000000);
        }
        error = HAL_CRU_SetPllFreq(&APLL, rate);
        if (rate < HAL_CRU_GetPllFreq(&APLL)) {
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(PCLK_DBG), rate / 150000000);
        }
        s_apllFreq = HAL_CRU_GetPllFreq(&APLL);

        return error;
    case DCLK_VOP:
        mux = 0;
        break;
    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_I2S0:
    case CLK_I2S1:
    case CLK_I2S2:
        error = HAL_CRU_ClkFracSetFreq(clockName, rate);

        return error;
    case PCLK_BUS:
    case HCLK_BUS:
        pRate = HAL_CRU_ClkGetFreq(ACLK_BUS);
        break;
    case PCLK_PERI:
    case HCLK_PERI:
        pRate = HAL_CRU_ClkGetFreq(ACLK_PERI);
        break;
    case CLK_TSADC:
    case CLK_SARADC:
    case CLK_CSI_OUT:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case CLK_SDMMC:
    case CLK_SDIO:
    case CLK_EMMC:
    case CLK_NANDC:
    case CLK_SFC:
        div = HAL_DIV_ROUND_UP(pRate, rate);
        if (div % 2) {
            div += 1;
        }
        break;
    case HCLK_VIO:
    case PCLK_VIO:
        pRate = HAL_CRU_ClkGetFreq(ACLK_VIO0);
        break;
    case CLK_CVBS_HOST:
        mux = 3;
        break;
    case CLK_HDMI_CEC:
        pRate = PLL_INPUT_OSC_RATE;
        mux = 2;
        break;
    case CLK_CIF_OUT:
        if (PLL_INPUT_OSC_RATE % rate) {
            HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_CIF_OUT_SRC), 0);
            HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_CIF_OUT_PLL), 1);
            div = HAL_DIV_ROUND_UP(pRate, rate);
        } else {
            HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_CIF_OUT_SRC), 1);
            div = 1;
        }
        HAL_CRU_ClkSetDiv(clkDiv, div);

        return 0;
    case CLK_DSP_PFM:
    case PCLK_DSP:
    case DSP_IOP:
    case DSP_EPP:
    case DSP_EDP:
    case DSP_EDAP:
        pRate = HAL_CRU_ClkGetFreq(CLK_DSP);
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

/** @} */

/** @} */

/** @} */

#endif /* SOC_RV1108 && HAL_CRU_MODULE_ENABLED */
