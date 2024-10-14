/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(SOC_RK1808) && defined(HAL_CRU_MODULE_ENABLED)

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
    RK_PLL_RATE(1000000000, 1, 125, 3, 1, 1, 0),
    RK_PLL_RATE(816000000, 1, 68, 2, 1, 1, 0),
    RK_PLL_RATE(200000000, 1, 200, 6, 4, 1, 0),
    RK_PLL_RATE(100000000, 1, 100, 6, 4, 1, 0),
    { /* sentinel */ },
};

static uint32_t s_apllFreq;
static uint32_t s_dpllFreq;
static uint32_t s_cpllFreq;
static uint32_t s_gpllFreq;
static uint32_t s_npllFreq;
static uint32_t s_ppllFreq;

static struct PLL_SETUP APLL = {
    .conOffset0 = &(CRU->APLL_CON[0]),
    .conOffset1 = &(CRU->APLL_CON[1]),
    .conOffset2 = &(CRU->APLL_CON[2]),
    .modeOffset = &(CRU->MODE),
    .modeShift = 0,
    .lockShift = 10,
    .modeMask = 0x3 << 0,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP DPLL = {
    .conOffset0 = &(CRU->DPLL_CON[0]),
    .conOffset1 = &(CRU->DPLL_CON[1]),
    .conOffset2 = &(CRU->DPLL_CON[2]),
    .modeOffset = &(CRU->MODE),
    .modeShift = 2,
    .lockShift = 10,
    .modeMask = 0x3 << 2,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP CPLL = {
    .conOffset0 = &(CRU->CPLL_CON[0]),
    .conOffset1 = &(CRU->CPLL_CON[1]),
    .conOffset2 = &(CRU->CPLL_CON[2]),
    .modeOffset = &(CRU->MODE),
    .modeShift = 4,
    .lockShift = 10,
    .modeMask = 0x3 << 4,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP GPLL = {
    .conOffset0 = &(CRU->GPLL_CON[0]),
    .conOffset1 = &(CRU->GPLL_CON[1]),
    .conOffset2 = &(CRU->GPLL_CON[2]),
    .modeOffset = &(CRU->MODE),
    .modeShift = 6,
    .lockShift = 10,
    .modeMask = 0x3 << 6,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP NPLL = {
    .conOffset0 = &(CRU->NPLL_CON[0]),
    .conOffset1 = &(CRU->NPLL_CON[1]),
    .conOffset2 = &(CRU->NPLL_CON[2]),
    .modeOffset = &(CRU->MODE),
    .modeShift = 8,
    .lockShift = 10,
    .modeMask = 0x3 << 8,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP PPLL = {
    .conOffset0 = &(CRU->PMUPLL_CON[0]),
    .conOffset1 = &(CRU->PMUPLL_CON[1]),
    .conOffset2 = &(CRU->PMUPLL_CON[2]),
    .modeOffset = &(CRU->PMU_MODE),
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
 * @brief  IP Clock set div API
 * @param  divName: div id(Contains div offset, shift, mask information)
 * @param  divValue: div value
 * @return NONE
 * @attention these APIs allow direct use in the HAL layer.
 */
HAL_Status HAL_PMUCRU_ClkSetDiv(uint32_t divName, uint32_t divValue)
{
    uint32_t shift, mask, index;

    index = CLK_DIV_GET_REG_OFFSET(divName);
    shift = CLK_DIV_GET_BITS_SHIFT(divName);
    HAL_ASSERT(shift < 16);
    mask = CLK_DIV_GET_MASK(divName);
    HAL_ASSERT(divValue < mask);

    CRU->PMU_CLKSEL_CON[index] = VAL_MASK_WE(mask, (divValue - 1U) << shift);

    return HAL_OK;
}

/**
 * @brief  IP Clock get div API
 * @param  divName: div id(Contains div offset, shift, mask information)
 * @return div value
 * @attention these APIs allow direct use in the HAL layer.
 */
uint32_t HAL_PMUCRU_ClkGetDiv(uint32_t divName)
{
    uint32_t shift, mask, index;

    index = CLK_DIV_GET_REG_OFFSET(divName);
    shift = CLK_DIV_GET_BITS_SHIFT(divName);
    HAL_ASSERT(shift < 16);
    mask = CLK_DIV_GET_MASK(divName);

    return ((((CRU->PMU_CLKSEL_CON[index]) & mask) >> shift) + 1);
}

/**
 * @brief  IP Clock set mux API
 * @param  muxName: mux id(Contains mux offset, shift, mask information)
 * @param  muxValue: mux value
 * @return NONE
 * @attention these APIs allow direct use in the HAL layer.
 */
HAL_Status HAL_PMUCRU_ClkSetMux(uint32_t muxName, uint32_t muxValue)
{
    uint32_t shift, mask, index;

    index = CLK_MUX_GET_REG_OFFSET(muxName);
    shift = CLK_MUX_GET_BITS_SHIFT(muxName);
    HAL_ASSERT(shift < 16);
    mask = CLK_MUX_GET_MASK(muxName);

    CRU->PMU_CLKSEL_CON[index] = VAL_MASK_WE(mask, muxValue << shift);

    return HAL_OK;
}

/**
 * @brief  IP Clock get mux API
 * @param  muxName: mux id(Contains mux offset, shift, mask information)
 * @return mux value
 * @attention these APIs allow direct use in the HAL layer.
 */
uint32_t HAL_PMUCRU_ClkGetMux(uint32_t muxName)
{
    uint32_t shift, mask, index;

    index = CLK_MUX_GET_REG_OFFSET(muxName);
    shift = CLK_MUX_GET_BITS_SHIFT(muxName);
    HAL_ASSERT(shift < 16);
    mask = CLK_MUX_GET_MASK(muxName);

    return ((CRU->PMU_CLKSEL_CON[index] & mask) >> shift);
}

/**
 * @brief Get vop dclk freq.
 * @param  clockName: CLOCK_Name id
 * @return clk rate.
 * @attention these APIs allow direct use in the HAL layer.
 * How to calculate the vop clk divider:
 *     if normal divider: rate = prate / div
 *     else frac divider:
 *     numerator is frac register[31:16]
 *     denominator is frac register[15:0]
 *     clk rate = pRate * numerator / denominator
 */
uint32_t HAL_CRU_ClkVopGetFreq(eCLOCK_Name clockName)
{
    uint32_t muxSrc, mux = CLK_GET_MUX(clockName);
    uint32_t divSrc, divFrac;
    uint32_t n = 0, m = 0, pRate = s_gpllFreq / 2, freq = 0;

    switch (clockName) {
    case DCLK_VOPRAW:
        muxSrc = CLK_GET_MUX(DCLK_VOPRAW_PLL);
        divSrc = CLK_GET_DIV(DCLK_VOPRAW_PLL);
        divFrac = CLK_GET_DIV(DCLK_VOPRAW_FRAC);
        break;
    case DCLK_VOPLITE:
        muxSrc = CLK_GET_MUX(DCLK_VOPLITE_PLL);
        divSrc = CLK_GET_DIV(DCLK_VOPLITE_PLL);
        divFrac = CLK_GET_DIV(DCLK_VOPLITE_FRAC);
        break;
    default:

        return HAL_INVAL;
    }

    n = (CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0xffff0000) >> 16;
    m = CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0x0000ffff;

    if (HAL_CRU_ClkGetMux(muxSrc) == 0) {
        pRate = s_cpllFreq / HAL_CRU_ClkGetDiv(divSrc);
    } else if (HAL_CRU_ClkGetMux(muxSrc) == 1) {
        pRate = s_gpllFreq / HAL_CRU_ClkGetDiv(divSrc);
    } else {
        pRate = s_npllFreq / HAL_CRU_ClkGetDiv(divSrc);
    }

    if (HAL_CRU_ClkGetMux(mux) == 0) {
        freq = pRate;
    } else if (HAL_CRU_ClkGetMux(mux) == 1) {
        freq = (pRate / m) * n;
    } else {
        freq = PLL_INPUT_OSC_RATE;
    }

    return freq;
}

/**
 * @brief Set vop dclk freq.
 * @param  clockName: CLOCK_Name id.
 * @param  rate: clk set rate
 * @return HAL_Status.
 * @attention these APIs allow direct use in the HAL layer.
 * How to calculate the vop clk divider:
 *     if gpll or cpll can aliquot rate, select normal divider
 *     else prate > 20 * rate, select frac divider
 *     else modify npll, make npll only for dclk_vop
 */
HAL_Status HAL_CRU_ClkVopSetFreq(eCLOCK_Name clockName, uint32_t rate)
{
    uint32_t muxSrc, mux = CLK_GET_MUX(clockName);
    uint32_t divSrc, divFrac;
    uint32_t n = 0, m = 0, pRate = s_gpllFreq / 2, div = 0, error = HAL_OK;

    switch (clockName) {
    case DCLK_VOPRAW:
        muxSrc = CLK_GET_MUX(DCLK_VOPRAW_PLL);
        divSrc = CLK_GET_DIV(DCLK_VOPRAW_PLL);
        divFrac = CLK_GET_DIV(DCLK_VOPRAW_FRAC);
        break;
    case DCLK_VOPLITE:
        muxSrc = CLK_GET_MUX(DCLK_VOPLITE_PLL);
        divSrc = CLK_GET_DIV(DCLK_VOPLITE_PLL);
        divFrac = CLK_GET_DIV(DCLK_VOPLITE_FRAC);
        break;
    default:

        return HAL_INVAL;
    }

    if (rate == PLL_INPUT_OSC_RATE) {
        HAL_CRU_ClkSetMux(mux, 2);
    } else if ((!(s_cpllFreq % rate))) {
        HAL_CRU_ClkSetDiv(divSrc, s_cpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        HAL_CRU_ClkSetMux(mux, 0);
    } else if ((!(s_gpllFreq % rate))) {
        HAL_CRU_ClkSetDiv(divSrc, s_gpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 1);
        HAL_CRU_ClkSetMux(mux, 0);
    } else if (pRate > 20 * rate) {
        HAL_CRU_FracdivGetConfig(rate, pRate, &n, &m);
        HAL_CRU_ClkSetDiv(divSrc, 2);
        HAL_CRU_ClkSetMux(muxSrc, 1);
        CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;
        HAL_CRU_ClkSetMux(mux, 1);
    } else {
        div = HAL_DIV_ROUND_UP(DCLK_LCDC_PLL_LIMIT_FREQ, rate);
        error = HAL_CRU_SetPllFreq(&NPLL, rate * div);
        s_npllFreq = HAL_CRU_GetPllFreq(&NPLL);
        HAL_CRU_ClkSetMux(muxSrc, 2);
        HAL_CRU_ClkSetMux(mux, 0);
    }

    return error;
}

/**
 * @brief Get npu clk freq.
 * @param  clockName: CLOCK_Name id
 * @return clk rate.
 * @attention these APIs allow direct use in the HAL layer.
 * How to calculate the Frac clk divider:
 *     if normal divider: rate = prate / div
 *     else np5 half divider:
 *     clk rate = pRate * 2 / ((div * 2) + 3)
 */
uint32_t HAL_CRU_ClkNpuGetFreq(eCLOCK_Name clockName)
{
    uint32_t mux = CLK_GET_MUX(clockName);
    uint32_t muxSrc = CLK_GET_MUX(CLK_NPU_D);
    uint32_t muxNp5 = CLK_GET_MUX(CLK_NPU_NP5);
    uint32_t divSrc = CLK_GET_DIV(CLK_NPU_D);
    uint32_t divNp5 = CLK_GET_DIV(CLK_NPU_NP5);
    uint32_t freq = 0, pRate = s_gpllFreq, div;

    if (HAL_CRU_ClkGetMux(mux)) {
        if (HAL_CRU_ClkGetMux(muxNp5)) {
            pRate = s_cpllFreq;
        }
        div = HAL_CRU_ClkGetDiv(divNp5);
        freq = (pRate * 2) / ((div - 1) * 2 + 3);
    } else {
        if (HAL_CRU_ClkGetMux(muxSrc)) {
            pRate = s_cpllFreq;
        }
        div = HAL_CRU_ClkGetDiv(divSrc);
        freq = pRate / div;
    }

    return freq;
}

/**
 * @brief Set npu clk freq.
 * @param  clockName: CLOCK_Name id.
 * @param  rate: clk set rate
 * @return HAL_Status.
 * @attention these APIs allow direct use in the HAL layer.
 * How to calculate the npu clk divider:
 *     if gpll or cpll can aliquot rate, select normal divider
 *     else select np5 half divider
 */
HAL_Status HAL_CRU_ClkNpuSetFreq(eCLOCK_Name clockName, uint32_t rate)
{
    uint32_t mux = CLK_GET_MUX(clockName);
    uint32_t muxSrc = CLK_GET_MUX(CLK_NPU_D);
    uint32_t muxNp5 = CLK_GET_MUX(CLK_NPU_NP5);
    uint32_t divSrc = CLK_GET_DIV(CLK_NPU_D);
    uint32_t divNp5 = CLK_GET_DIV(CLK_NPU_NP5);
    uint32_t oldRate = HAL_CRU_ClkNpuGetFreq(clockName);
    uint32_t div = 0, selNp5 = 0, selPll = 0;

    if (rate == 666666666) {
        div = 0;
        selPll = 1;
        selNp5 = 1;
    } else if (rate == 285714285) {
        div = 2;
        selPll = 1;
        selNp5 = 1;
    } else if (rate == 339428571) {
        div = 2;
        selPll = 0;
        selNp5 = 1;
    } else if (HAL_CRU_ClkNp5BestDiv(CLK_NPU_NP5, rate, s_cpllFreq, &div) == 0) {
        selPll = 1;
        selNp5 = 1;
    } else if (HAL_CRU_ClkNp5BestDiv(CLK_NPU_NP5, rate, s_gpllFreq, &div) == 0) {
        selPll = 0;
        selNp5 = 1;
    } else if (!(s_cpllFreq % rate)) {
        div = HAL_DIV_ROUND_UP(s_cpllFreq, rate);
        selPll = 1;
        selNp5 = 0;
    } else {
        div = HAL_DIV_ROUND_UP(s_gpllFreq, rate);
        selPll = 0;
        selNp5 = 0;
    }
    if (selNp5) {
        if (rate > oldRate) {
            HAL_CRU_ClkSetDiv(divNp5, (div + 1));
        }
        HAL_CRU_ClkSetMux(muxNp5, selPll);
        HAL_CRU_ClkSetMux(mux, selNp5);
        if (rate <= oldRate) {
            HAL_CRU_ClkSetDiv(divNp5, (div + 1));
        }
    } else {
        if (rate > oldRate) {
            HAL_CRU_ClkSetDiv(divSrc, div);
        }
        HAL_CRU_ClkSetMux(muxSrc, selPll);
        HAL_CRU_ClkSetMux(mux, selNp5);
        if (rate <= oldRate) {
            HAL_CRU_ClkSetDiv(divSrc, div);
        }
    }

    return HAL_OK;
}

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
    uint32_t muxSrc, mux = CLK_GET_MUX(clockName);
    uint32_t divSrc, divFrac;
    uint32_t n, m, pRate = s_gpllFreq, is_cru = 1, is_i2s = 0;

    switch (clockName) {
    case CLK_UART0:
        muxSrc = CLK_GET_MUX(CLK_UART0_PLL);
        divSrc = CLK_GET_DIV(CLK_UART0_PLL);
        divFrac = CLK_GET_DIV(CLK_UART0_FRAC);
        is_cru = 0;
        is_i2s = 0;
        break;
    case CLK_UART1:
        muxSrc = CLK_GET_MUX(CLK_UART1_PLL);
        divSrc = CLK_GET_DIV(CLK_UART1_PLL);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        is_cru = 1;
        is_i2s = 0;
        break;
    case CLK_UART2:
        muxSrc = CLK_GET_MUX(CLK_UART2_PLL);
        divSrc = CLK_GET_DIV(CLK_UART2_PLL);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        is_cru = 1;
        is_i2s = 0;
        break;
    case CLK_UART3:
        muxSrc = CLK_GET_MUX(CLK_UART3_PLL);
        divSrc = CLK_GET_DIV(CLK_UART3_PLL);
        divFrac = CLK_GET_DIV(CLK_UART3_FRAC);
        is_cru = 1;
        is_i2s = 0;
        break;
    case CLK_UART4:
        muxSrc = CLK_GET_MUX(CLK_UART4_PLL);
        divSrc = CLK_GET_DIV(CLK_UART4_PLL);
        divFrac = CLK_GET_DIV(CLK_UART4_FRAC);
        is_cru = 1;
        is_i2s = 0;
        break;
    case CLK_UART5:
        muxSrc = CLK_GET_MUX(CLK_UART5_PLL);
        divSrc = CLK_GET_DIV(CLK_UART5_PLL);
        divFrac = CLK_GET_DIV(CLK_UART5_FRAC);
        is_cru = 1;
        is_i2s = 0;
        break;
    case CLK_UART6:
        muxSrc = CLK_GET_MUX(CLK_UART6_PLL);
        divSrc = CLK_GET_DIV(CLK_UART6_PLL);
        divFrac = CLK_GET_DIV(CLK_UART6_FRAC);
        is_cru = 1;
        is_i2s = 0;
        break;
    case CLK_UART7:
        muxSrc = CLK_GET_MUX(CLK_UART7_PLL);
        divSrc = CLK_GET_DIV(CLK_UART7_PLL);
        divFrac = CLK_GET_DIV(CLK_UART7_FRAC);
        break;
    case CLK_I2S0_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S0_RX_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S0_RX_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S0_RX_FRAC);
        is_cru = 1;
        is_i2s = 1;
        break;
    case CLK_I2S0_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S0_TX_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S0_TX_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S0_TX_FRAC);
        is_cru = 1;
        is_i2s = 1;
        break;
    case CLK_I2S1:
        muxSrc = CLK_GET_MUX(CLK_I2S1_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S1_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S1_FRAC);
        is_cru = 1;
        is_i2s = 1;
        break;
    default:

        return HAL_INVAL;
    }

    if (is_cru) {
        n = (CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0xffff0000) >> 16;
        m = CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0x0000ffff;

        if (HAL_CRU_ClkGetMux(muxSrc)) {
            pRate = s_cpllFreq / HAL_CRU_ClkGetDiv(divSrc);
        } else {
            pRate = s_gpllFreq / HAL_CRU_ClkGetDiv(divSrc);
        }

        if (is_i2s) {
            if (HAL_CRU_ClkGetMux(mux) == 0) {
                freq = pRate;
            } else if (HAL_CRU_ClkGetMux(mux) == 1) {
                freq = (pRate / m) * n;
            } else if (HAL_CRU_ClkGetMux(mux) == 3) {
                freq = PLL_INPUT_OSC_RATE / 2;
            }
        } else {
            if (HAL_CRU_ClkGetMux(mux) == 0) {
                freq = pRate;
            } else if (HAL_CRU_ClkGetMux(mux) == 2) {
                freq = (pRate / m) * n;
            } else if (HAL_CRU_ClkGetMux(mux) == 3) {
                freq = PLL_INPUT_OSC_RATE;
            }
        }
    } else {
        n = (CRU->PMU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0xffff0000) >> 16;
        m = CRU->PMU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0x0000ffff;

        if (HAL_PMUCRU_ClkGetMux(muxSrc)) {
            pRate = s_cpllFreq / HAL_PMUCRU_ClkGetDiv(divSrc);
        } else {
            pRate = s_gpllFreq / HAL_PMUCRU_ClkGetDiv(divSrc);
        }

        if (HAL_PMUCRU_ClkGetMux(mux) == 0) {
            freq = pRate;
        } else if (HAL_PMUCRU_ClkGetMux(mux) == 2) {
            freq = (pRate / m) * n;
        } else if (HAL_PMUCRU_ClkGetMux(mux) == 3) {
            freq = PLL_INPUT_OSC_RATE;
        }
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
    uint32_t muxSrc, mux = CLK_GET_MUX(clockName);
    uint32_t divSrc, divFrac;
    uint32_t n = 0, m = 0, pRate = s_gpllFreq / 2, is_cru = 1, is_i2s = 0;

    switch (clockName) {
    case CLK_UART0:
        muxSrc = CLK_GET_MUX(CLK_UART0_PLL);
        divSrc = CLK_GET_DIV(CLK_UART0_PLL);
        divFrac = CLK_GET_DIV(CLK_UART0_FRAC);
        is_cru = 0;
        is_i2s = 0;
        break;
    case CLK_UART1:
        muxSrc = CLK_GET_MUX(CLK_UART1_PLL);
        divSrc = CLK_GET_DIV(CLK_UART1_PLL);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        is_cru = 1;
        is_i2s = 0;
        break;
    case CLK_UART2:
        muxSrc = CLK_GET_MUX(CLK_UART2_PLL);
        divSrc = CLK_GET_DIV(CLK_UART2_PLL);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        is_cru = 1;
        is_i2s = 0;
        break;
    case CLK_UART3:
        muxSrc = CLK_GET_MUX(CLK_UART3_PLL);
        divSrc = CLK_GET_DIV(CLK_UART3_PLL);
        divFrac = CLK_GET_DIV(CLK_UART3_FRAC);
        is_cru = 1;
        is_i2s = 0;
        break;
    case CLK_UART4:
        muxSrc = CLK_GET_MUX(CLK_UART4_PLL);
        divSrc = CLK_GET_DIV(CLK_UART4_PLL);
        divFrac = CLK_GET_DIV(CLK_UART4_FRAC);
        is_cru = 1;
        is_i2s = 0;
        break;
    case CLK_UART5:
        muxSrc = CLK_GET_MUX(CLK_UART5_PLL);
        divSrc = CLK_GET_DIV(CLK_UART5_PLL);
        divFrac = CLK_GET_DIV(CLK_UART5_FRAC);
        is_cru = 1;
        is_i2s = 0;
        break;
    case CLK_UART6:
        muxSrc = CLK_GET_MUX(CLK_UART6_PLL);
        divSrc = CLK_GET_DIV(CLK_UART6_PLL);
        divFrac = CLK_GET_DIV(CLK_UART6_FRAC);
        is_cru = 1;
        is_i2s = 0;
        break;
    case CLK_UART7:
        muxSrc = CLK_GET_MUX(CLK_UART7_PLL);
        divSrc = CLK_GET_DIV(CLK_UART7_PLL);
        divFrac = CLK_GET_DIV(CLK_UART7_FRAC);
        break;
    case CLK_I2S0_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S0_RX_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S0_RX_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S0_RX_FRAC);
        is_cru = 1;
        is_i2s = 1;
        break;
    case CLK_I2S0_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S0_TX_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S0_TX_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S0_TX_FRAC);
        is_cru = 1;
        is_i2s = 1;
        break;
    case CLK_I2S1:
        muxSrc = CLK_GET_MUX(CLK_I2S1_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S1_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S1_FRAC);
        is_cru = 1;
        is_i2s = 1;
        break;
    default:

        return HAL_INVAL;
    }

    if (is_cru) {
        if ((!(s_gpllFreq % rate)) && ((s_gpllFreq / rate) < 31)) {
            HAL_CRU_ClkSetDiv(divSrc, s_gpllFreq / rate);
            HAL_CRU_ClkSetMux(muxSrc, 0);
            HAL_CRU_ClkSetMux(mux, 0);
        } else {
            HAL_CRU_FracdivGetConfig(rate, pRate, &n, &m);
            HAL_CRU_ClkSetDiv(divSrc, 2);
            HAL_CRU_ClkSetMux(muxSrc, 0);
            CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;
            if (is_i2s) {
                HAL_CRU_ClkSetMux(mux, 1);
            } else {
                HAL_CRU_ClkSetMux(mux, 2);
            }
        }
    } else {
        if ((!(s_gpllFreq % rate)) && ((s_gpllFreq / rate) < 31)) {
            HAL_PMUCRU_ClkSetDiv(divSrc, s_gpllFreq / rate);
            HAL_PMUCRU_ClkSetMux(muxSrc, 0);
            HAL_PMUCRU_ClkSetMux(mux, 0);
        } else {
            HAL_CRU_FracdivGetConfig(rate, pRate, &n, &m);
            HAL_PMUCRU_ClkSetDiv(divSrc, 2);
            HAL_PMUCRU_ClkSetMux(muxSrc, 0);
            CRU->PMU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;
            HAL_PMUCRU_ClkSetMux(mux, 2);
        }
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
    uint32_t pRate = s_gpllFreq, is_cru = 1;

    if (!s_gpllFreq) {
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    }
    if (!s_cpllFreq) {
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);
    }
    if (!s_npllFreq) {
        s_npllFreq = HAL_CRU_GetPllFreq(&NPLL);
    }
    if (!s_ppllFreq) {
        s_ppllFreq = HAL_CRU_GetPllFreq(&PPLL);
    }

    switch (clockName) {
    case PLL_DPLL:
        freq = HAL_CRU_GetPllFreq(&DPLL);
        s_dpllFreq = freq;

        return freq;
    case PLL_GPLL:
        freq = HAL_CRU_GetPllFreq(&GPLL);
        s_gpllFreq = freq;

        return freq;
    case PLL_CPLL:
        freq = HAL_CRU_GetPllFreq(&CPLL);
        s_cpllFreq = freq;

        return freq;
    case PLL_NPLL:
        freq = HAL_CRU_GetPllFreq(&NPLL);
        s_npllFreq = freq;

        return freq;
    case PLL_PPLL:
        freq = HAL_CRU_GetPllFreq(&PPLL);
        s_ppllFreq = freq;

        return freq;
    case PLL_APLL:
    case CLK_CPU:
        freq = HAL_CRU_GetPllFreq(&APLL);
        s_apllFreq = freq;

        return freq;
    case DCLK_VOPRAW:
    case DCLK_VOPLITE:
        freq = HAL_CRU_ClkVopGetFreq(clockName);

        return freq;
    case CLK_NPU:
        freq = HAL_CRU_ClkNpuGetFreq(clockName);

        return freq;
    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
    case CLK_UART6:
    case CLK_UART7:
    case CLK_I2S0_RX:
    case CLK_I2S0_TX:
    case CLK_I2S1:
        freq = HAL_CRU_ClkFracGetFreq(clockName);

        return freq;
    case DBCLK_GPIO1:
    case DBCLK_GPIO2:
    case DBCLK_GPIO3:
    case DBCLK_GPIO4:
    case CLK_TSADC:
    case CLK_SARADC:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case ACLK_NPU2MEM:
        pRate = HAL_CRU_ClkGetFreq(ACLK_NPU);
        break;
    case LSCLK_VIO:
        pRate = HAL_CRU_ClkGetFreq(HSCLK_VIO);
        break;
    case CLK_USB3_OTG_SUSPEND:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = PLL_INPUT_OSC_RATE;
        } else {
            pRate = 32768;
        }
        break;
    case PCLK_PCIE:
        pRate = HAL_CRU_ClkGetFreq(ACLK_PCIE);
        break;
    case PCLK_VPU:
        pRate = HAL_CRU_ClkGetFreq(ACLK_VPU);
        break;
    case CLK_CIF_OUT:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            pRate = PLL_INPUT_OSC_RATE;
        } else {
            pRate = s_gpllFreq;
        }
        break;
    case DCLK_CIF:
    case CLK_PCIE_AUX_PLL:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = s_gpllFreq;
        } else {
            pRate = s_cpllFreq;
        }
        break;
    case CLK_GMAC_OUT:
    case CLK_GMAC:
        pRate = s_cpllFreq;
        break;
    case PCLK_PDPMU:
    case XIN_OSC0_FUNC:
    case CLK_REF24M:
    case CLK_I2C0:
        pRate = s_ppllFreq;
        is_cru = 0;
        break;
    case DBCLK_GPIO0:
        pRate = PLL_INPUT_OSC_RATE;
        is_cru = 0;
        break;
    case CLK_PCIEPHY_REF_CLK:
        if (!HAL_PMUCRU_ClkGetMux(clkMux)) {
            freq = PLL_INPUT_OSC_RATE;
        } else {
            freq = s_ppllFreq / HAL_PMUCRU_ClkGetDiv(clkDiv);
        }

        return freq;
    case CLK_WIFI:
        if (HAL_PMUCRU_ClkGetMux(clkMux)) {
            freq = PLL_INPUT_OSC_RATE;
        } else {
            freq = s_ppllFreq / HAL_PMUCRU_ClkGetDiv(CLK_GET_DIV(CLK_WIFI_PLL));
        }

        return freq;
    default:
        break;
    }

    if ((clkMux == 0) && (clkDiv == 0)) {
        return 0;
    }

    if (is_cru) {
        if (clkDiv) {
            freq = pRate / (HAL_CRU_ClkGetDiv(clkDiv));
        } else {
            freq = pRate;
        }
    } else {
        if (clkDiv) {
            freq = pRate / (HAL_PMUCRU_ClkGetDiv(clkDiv));
        } else {
            freq = pRate;
        }
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
    uint32_t pRate = s_gpllFreq, is_cru = 1;

    if (rate == HAL_CRU_ClkGetFreq(clockName)) {
        return HAL_OK;
    }

    if (!s_gpllFreq) {
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    }
    if (!s_cpllFreq) {
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);
    }
    if (!s_npllFreq) {
        s_npllFreq = HAL_CRU_GetPllFreq(&NPLL);
    }
    if (!s_ppllFreq) {
        s_ppllFreq = HAL_CRU_GetPllFreq(&PPLL);
    }

    switch (clockName) {
    case PLL_DPLL:
        error = HAL_CRU_SetPllFreq(&DPLL, rate);
        s_dpllFreq = HAL_CRU_GetPllFreq(&DPLL);

        return error;
    case PLL_GPLL:
        error = HAL_CRU_SetPllFreq(&GPLL, rate);
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);

        return error;
    case PLL_CPLL:
        error = HAL_CRU_SetPllFreq(&CPLL, rate);
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);

        return error;
    case PLL_NPLL:
        error = HAL_CRU_SetPllFreq(&NPLL, rate);
        s_npllFreq = HAL_CRU_GetPllFreq(&NPLL);

        return error;
    case PLL_PPLL:
        error = HAL_CRU_SetPllFreq(&PPLL, rate);
        s_ppllFreq = HAL_CRU_GetPllFreq(&PPLL);

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
    case DCLK_VOPRAW:
    case DCLK_VOPLITE:
        error = HAL_CRU_ClkVopSetFreq(clockName, rate);

        return error;
    case CLK_NPU:
        error = HAL_CRU_ClkNpuSetFreq(clockName, rate);

        return error;
    case CLK_PDM:
    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
    case CLK_UART6:
    case CLK_UART7:
    case CLK_I2S0_RX:
    case CLK_I2S0_TX:
    case CLK_I2S1:
        error = HAL_CRU_ClkFracSetFreq(clockName, rate);

        return error;
    case DBCLK_GPIO1:
    case DBCLK_GPIO2:
    case DBCLK_GPIO3:
    case DBCLK_GPIO4:
    case CLK_TSADC:
    case CLK_SARADC:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case CLK_SDIO_PLL:
    case CLK_EMMC_PLL:
    case CLK_SDMMC_PLL:
        HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_SDIO), 1);
        HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_EMMC), 1);
        HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_SDMMC), 1);
        break;
    case ACLK_NPU2MEM:
        pRate = HAL_CRU_ClkGetFreq(ACLK_NPU);
        break;
    case LSCLK_VIO:
        pRate = HAL_CRU_ClkGetFreq(HSCLK_VIO);
        break;
    case CLK_USB3_OTG_SUSPEND:
        if (rate == 32768) {
            mux = 0;
            pRate = 32768;
        } else {
            mux = 1;
            pRate = PLL_INPUT_OSC_RATE;
        }
        break;
    case PCLK_PCIE:
        pRate = HAL_CRU_ClkGetFreq(ACLK_PCIE);
        break;
    case PCLK_VPU:
        pRate = HAL_CRU_ClkGetFreq(ACLK_VPU);
        break;
    case CLK_CIF_OUT:
        if (rate == PLL_INPUT_OSC_RATE) {
            mux = 0;
            pRate = PLL_INPUT_OSC_RATE;
        } else {
            mux = 2;
        }
        break;
    case DCLK_CIF:
    case CLK_PCIE_AUX_PLL:
        if (!(s_cpllFreq % rate)) {
            mux = 0;
            pRate = s_cpllFreq;
        } else {
            mux = 1;
        }
        break;
    case CLK_GMAC_OUT:
    case CLK_GMAC:
        pRate = s_cpllFreq;
        break;
    case PCLK_PDPMU:
    case XIN_OSC0_FUNC:
    case CLK_REF24M:
    case CLK_I2C0:
        pRate = s_ppllFreq;
        is_cru = 0;
        break;
    case DBCLK_GPIO0:
        pRate = PLL_INPUT_OSC_RATE;
        is_cru = 0;
        break;
    case CLK_PCIEPHY_REF_CLK:
        if (rate == PLL_INPUT_OSC_RATE) {
            mux = 0;
        } else {
            mux = 1;
            div = HAL_DIV_ROUND_UP(s_ppllFreq, rate);
            HAL_PMUCRU_ClkSetDiv(clkDiv, div);
        }

        return HAL_OK;
    case CLK_WIFI:
        if (rate == PLL_INPUT_OSC_RATE) {
            mux = 1;
        } else {
            mux = 0;
            div = HAL_DIV_ROUND_UP(s_ppllFreq, rate);
            HAL_PMUCRU_ClkSetDiv(CLK_GET_DIV(CLK_WIFI_PLL), div);
        }

        return HAL_OK;
    default:
        break;
    }

    if ((clkMux == 0) && (clkDiv == 0)) {
        return HAL_INVAL;
    }

    div = HAL_DIV_ROUND_UP(pRate, rate);

    if (is_cru) {
        if (clkMux) {
            HAL_CRU_ClkSetMux(clkMux, mux);
        }
        if (clkDiv) {
            HAL_CRU_ClkSetDiv(clkDiv, div);
        }
    } else {
        if (clkMux) {
            HAL_PMUCRU_ClkSetMux(clkMux, mux);
        }
        if (clkDiv) {
            HAL_PMUCRU_ClkSetDiv(clkDiv, div);
        }
    }

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* SOC_RK1808 && HAL_CRU_MODULE_ENABLED */
