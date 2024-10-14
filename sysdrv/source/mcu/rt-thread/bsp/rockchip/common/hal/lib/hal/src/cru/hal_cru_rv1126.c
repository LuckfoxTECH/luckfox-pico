/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(SOC_RV1126) && defined(HAL_CRU_MODULE_ENABLED)

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

#define OSC_32KHZ     32768
#define PCLK_DBG_RATE (150 * MHZ)

/********************* Private Structure Definition **************************/

static struct PLL_CONFIG PLL_TABLE[] = {
    /* _mhz, _refDiv, _fbDiv, _postdDv1, _postDiv2, _dsmpd, _frac */
    RK_PLL_RATE(1200000000, 1, 100, 2, 1, 1, 0),
    RK_PLL_RATE(1188000000, 1, 99, 2, 1, 1, 0),
    RK_PLL_RATE(1008000000, 1, 84, 2, 1, 1, 0),
    RK_PLL_RATE(1000000000, 3, 250, 2, 1, 1, 0),
    RK_PLL_RATE(816000000, 1, 68, 2, 1, 1, 0),
    RK_PLL_RATE(600000000, 1, 100, 4, 1, 1, 0),
    { /* sentinel */ },
};

static uint32_t s_apllFreq;
static uint32_t s_cpllFreq;
static uint32_t s_hpllFreq;
static uint32_t s_gpllFreq;

static struct PLL_SETUP APLL = {
    .conOffset0 = &(CRU->APLL_CON[0]),
    .conOffset1 = &(CRU->APLL_CON[1]),
    .conOffset2 = &(CRU->APLL_CON[2]),
    .conOffset3 = &(CRU->APLL_CON[3]),
    .modeOffset = &(CRU->MODE_CON01),
    .modeShift = 0,
    .lockShift = 10,
    .modeMask = 0x3,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP CPLL = {
    .conOffset0 = &(CRU->CPLL_CON[0]),
    .conOffset1 = &(CRU->CPLL_CON[1]),
    .conOffset2 = &(CRU->CPLL_CON[2]),
    .conOffset3 = &(CRU->CPLL_CON[3]),
    .modeOffset = &(CRU->MODE_CON01),
    .modeShift = 4,
    .lockShift = 10,
    .modeMask = 0x3 << 4,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP HPLL = {
    .conOffset0 = &(CRU->HPLL_CON[0]),
    .conOffset1 = &(CRU->HPLL_CON[1]),
    .conOffset2 = &(CRU->HPLL_CON[2]),
    .conOffset3 = &(CRU->HPLL_CON[3]),
    .modeOffset = &(CRU->MODE_CON01),
    .modeShift = 6,
    .lockShift = 10,
    .modeMask = 0x3 << 6,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP GPLL = {
    .conOffset0 = &(PMUCRU->GPLL_CON[0]),
    .conOffset1 = &(PMUCRU->GPLL_CON[1]),
    .conOffset2 = &(PMUCRU->GPLL_CON[2]),
    .conOffset3 = &(PMUCRU->GPLL_CON[3]),
    .modeOffset = &(PMUCRU->MODE_CON00),
    .modeShift = 0,
    .lockShift = 10,
    .modeMask = 0x3,
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
    uint32_t index;

    switch (clockName) {
    case CLK_OSC0_DIV32K:
    case CLK_RTC32K:
        pRate = PLL_INPUT_OSC_RATE;
        divFrac = CLK_GET_DIV(CLK_OSC0_DIV32K);
        index = CLK_DIV_GET_REG_OFFSET(divFrac) - CRU_CLK_DIV_CON_CNT;
        n = (PMUCRU->CRU_CLKSEL_CON[index] & 0xffff0000) >> 16;
        m = PMUCRU->CRU_CLKSEL_CON[index] & 0x0000ffff;
        freq = (pRate / m) * n;

        return freq;
    case CLK_UART2:
        muxSrc = CLK_GET_MUX(CLK_UART2_SRC);
        divSrc = CLK_GET_DIV(CLK_UART2_SRC);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        break;
    default:

        return HAL_INVAL;
    }

    n = (CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0xffff0000) >> 16;
    m = CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0x0000ffff;

    if (HAL_CRU_ClkGetMux(muxSrc)) {
        pRate = s_cpllFreq / HAL_CRU_ClkGetDiv(divSrc);
    } else {
        pRate = s_gpllFreq / HAL_CRU_ClkGetDiv(divSrc);
    }

    if (HAL_CRU_ClkGetMux(mux) == SCLK_UART2_SEL_SCLK_UART2_DIV) {
        freq = pRate;
    } else if (HAL_CRU_ClkGetMux(mux) == SCLK_UART2_SEL_SCLK_UART2_FRAC) {
        freq = (pRate / m) * n;
    } else if (HAL_CRU_ClkGetMux(mux) == SCLK_UART2_SEL_XIN_OSC0_FUNC) {
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
    uint32_t index;

    switch (clockName) {
    case CLK_OSC0_DIV32K:
    case CLK_RTC32K:
        HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_RTC32K),
                          CLK_DEEPSLOW_SEL_CLK_OSC0_DIV32K);

        divFrac = CLK_GET_DIV(CLK_OSC0_DIV32K);
        index = CLK_DIV_GET_REG_OFFSET(divFrac) - CRU_CLK_DIV_CON_CNT;
        HAL_CRU_FracdivGetConfig(rate, PLL_INPUT_OSC_RATE, &n, &m);
        PMUCRU->CRU_CLKSEL_CON[index] = (n << 16) | m;

        return HAL_OK;
    case CLK_UART2:
        if (rate == PLL_INPUT_OSC_RATE) {
            HAL_CRU_ClkSetMux(mux, SCLK_UART2_SEL_XIN_OSC0_FUNC);
            HAL_CRU_ClkDisable(SCLK_UART2_DIV_GATE);

            return HAL_OK;
        }
        muxSrc = CLK_GET_MUX(CLK_UART2_SRC);
        divSrc = CLK_GET_DIV(CLK_UART2_SRC);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        gateId = SCLK_UART2_DIV_GATE;
        fracGateId = SCLK_UART2_FRAC_GATE;
        break;
    default:

        return HAL_INVAL;
    }

    HAL_CRU_ClkEnable(gateId);
    HAL_CRU_ClkEnable(fracGateId);

    if ((!(s_cpllFreq % rate)) && ((s_cpllFreq / rate) < 16)) {
        HAL_CRU_ClkSetDiv(divSrc, s_cpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, SCLK_UART2_DIV_SEL_CLK_GPLL_MUX);
        HAL_CRU_ClkSetMux(mux, SCLK_UART2_SEL_SCLK_UART2_DIV);
        HAL_CRU_ClkDisable(fracGateId);
    } else {
        HAL_CRU_FracdivGetConfig(rate, s_cpllFreq, &n, &m);
        HAL_CRU_ClkSetDiv(divSrc, 1);
        HAL_CRU_ClkSetMux(muxSrc, SCLK_UART2_DIV_SEL_CLK_GPLL_MUX);
        HAL_CRU_ClkSetMux(mux, SCLK_UART2_SEL_SCLK_UART2_FRAC);
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
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t pRate = 0, sel, freq;

    if (!s_cpllFreq) {
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);
    }
    if (!s_gpllFreq) {
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    }

    switch (clockName) {
    case PLL_APLL:
    case ARMCLK:
        freq = HAL_CRU_GetPllFreq(&APLL);
        s_apllFreq = freq;

        return freq;
    case PLL_CPLL:
        freq = HAL_CRU_GetPllFreq(&CPLL);
        s_cpllFreq = freq;

        return freq;
    case PLL_HPLL:
        freq = HAL_CRU_GetPllFreq(&HPLL);
        s_hpllFreq = freq;

        return freq;
    case PLL_GPLL:
        freq = HAL_CRU_GetPllFreq(&GPLL);
        s_gpllFreq = freq;

        return freq;
    case HCLK_PDCORE:
    case HCLK_PDPHP:
    case HCLK_PDAUDIO:
    case PCLK_PDPMU:
    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_I2C2:
    case CLK_I2C3:
    case CLK_I2C4:
    case CLK_I2C5:
        pRate = s_gpllFreq;
        break;
    case ACLK_PDBUS:
    case HCLK_PDBUS:
    case PCLK_PDBUS:
    case ACLK_PDPHP:
    case ACLK_PDVO:
    case DCLK_VOP:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = s_cpllFreq;
        } else {
            pRate = s_gpllFreq;
        }
        break;
    case CLK_SPI0:
    case CLK_SPI1:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = PLL_INPUT_OSC_RATE;
        } else {
            pRate = s_gpllFreq;
        }
        break;
    case ACLK_PDVI:
    case ACLK_PDISPP:
    case CLK_ISP:
    case CLK_ISPP:
        sel = HAL_CRU_ClkGetMux(clkMux);
        if (sel == ACLK_PDVI_DIV_SEL_CLK_CPLL_MUX) {
            pRate = s_cpllFreq;
        } else if (sel == ACLK_PDVI_DIV_SEL_CLK_GPLL_MUX) {
            pRate = s_gpllFreq;
        } else {
            pRate = s_hpllFreq;
        }
        break;
    case CLK_OSC0_DIV32K:
    case CLK_RTC32K:
    case CLK_UART2:
        freq = HAL_CRU_ClkFracGetFreq(clockName);

        return freq;
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
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t mux = 0, div = 0, pRate = 0;

    if (!s_cpllFreq) {
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);
    }
    if (!s_gpllFreq) {
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    }

    switch (clockName) {
    case PLL_APLL:
    case ARMCLK:
        s_apllFreq = HAL_CRU_GetPllFreq(&APLL);
        if (rate > s_apllFreq) {
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(PCLK_DBG), rate / PCLK_DBG_RATE);
        }
        error = HAL_CRU_SetPllFreq(&APLL, rate);
        if (error != HAL_OK) {
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(PCLK_DBG), s_apllFreq / PCLK_DBG_RATE);

            return error;
        }
        if (rate < s_apllFreq) {
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(PCLK_DBG), rate / PCLK_DBG_RATE);
        }
        s_apllFreq = HAL_CRU_GetPllFreq(&APLL);

        return error;
    case PLL_CPLL:
        error = HAL_CRU_SetPllFreq(&CPLL, rate);
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);

        return error;
    case PLL_HPLL:
        error = HAL_CRU_SetPllFreq(&HPLL, rate);
        s_hpllFreq = HAL_CRU_GetPllFreq(&HPLL);

        return error;
    case PLL_GPLL:
        error = HAL_CRU_SetPllFreq(&GPLL, rate);
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);

        return error;
    case ACLK_PDBUS:
        pRate = s_cpllFreq;
        mux = ACLK_PDBUS_PRE_SEL_CLK_CPLL_MUX;
        break;
    case HCLK_PDBUS:
    case PCLK_PDBUS:
    case ACLK_PDPHP:
    case HCLK_PDPHP:
    case HCLK_PDCORE:
    case HCLK_PDAUDIO:
    case PCLK_PDPMU:
    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_I2C2:
    case CLK_I2C3:
    case CLK_I2C4:
    case CLK_I2C5:
    case ACLK_PDVO:
        pRate = s_gpllFreq;
        mux = HCLK_PDBUS_PRE_SEL_CLK_GPLL_MUX;
        break;
    case CLK_SPI0:
    case CLK_SPI1:
        if (rate == PLL_INPUT_OSC_RATE) {
            pRate = PLL_INPUT_OSC_RATE;
            mux = CLK_SPI0_SEL_XIN_OSC0_FUNC;
        } else {
            pRate = s_gpllFreq;
            mux = CLK_SPI0_SEL_CLK_GPLL_MUX;
        }
        break;
    case CLK_OSC0_DIV32K:
    case CLK_RTC32K:
        error = HAL_CRU_ClkFracSetFreq(clockName, rate);

        return error;
    case DCLK_VOP:
        if (!(s_gpllFreq % rate)) {
            pRate = s_gpllFreq;
            mux = DCLK_VOP_DIV_SEL_CLK_GPLL_MUX;
        } else {
            pRate = s_cpllFreq;
            mux = DCLK_VOP_DIV_SEL_CLK_CPLL_MUX;
        }
        break;
    case ACLK_PDVI:
    case ACLK_PDISPP:
    case CLK_ISP:
    case CLK_ISPP:
        if (!(s_cpllFreq % rate)) {
            pRate = s_cpllFreq;
            mux = ACLK_PDVI_DIV_SEL_CLK_CPLL_MUX;
        } else {
            pRate = s_gpllFreq;
            mux = ACLK_PDVI_DIV_SEL_CLK_GPLL_MUX;
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
    uint32_t mask = CRU_GLB_RST_CON_CRU_WDT_EN_MASK | CRU_GLB_RST_CON_CRU_WDT_CON_MASK;
    uint32_t val = 1 << CRU_GLB_RST_CON_CRU_WDT_EN_SHIFT;

    switch (wdtType) {
    case GLB_RST_FST_WDT0:
        val |= (1 << CRU_GLB_RST_CON_CRU_WDT_CON_SHIFT);
        break;
    case GLB_RST_SND_WDT0:
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

#endif /* SOC_RV1126 && HAL_CRU_MODULE_ENABLED */
