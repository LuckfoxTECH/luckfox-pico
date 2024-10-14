/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */
#include "hal_base.h"

#if defined(SOC_RK3562) && defined(HAL_CRU_MODULE_ENABLED)

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

/********************* Private Variable Definition ***************************/

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

static uint32_t s_gpllFreq;
static uint32_t s_cpllFreq;

static struct PLL_SETUP APLL = {
    .conOffset0 = &(TOPCRU->APLL_CON[0]),
    .conOffset1 = &(TOPCRU->APLL_CON[1]),
    .conOffset2 = &(TOPCRU->APLL_CON[2]),
    .modeOffset = &(TOPCRU->MODE_CON00),
    .modeShift = 0,
    .lockShift = 10,
    .modeMask = 0x3,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP GPLL = {
    .conOffset0 = &(TOPCRU->GPLL_CON[0]),
    .conOffset1 = &(TOPCRU->GPLL_CON[1]),
    .conOffset2 = &(TOPCRU->GPLL_CON[2]),
    .conOffset3 = &(TOPCRU->GPLL_CON[3]),
    .modeOffset = &(TOPCRU->MODE_CON00),
    .modeShift = 2,
    .lockShift = 10,
    .modeMask = 0x3 << 2,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP VPLL = {
    .conOffset0 = &(TOPCRU->VPLL_CON[0]),
    .conOffset1 = &(TOPCRU->VPLL_CON[1]),
    .conOffset2 = &(TOPCRU->VPLL_CON[2]),
    .conOffset3 = &(TOPCRU->VPLL_CON[3]),
    .modeOffset = &(TOPCRU->MODE_CON00),
    .modeShift = 6,
    .lockShift = 10,
    .modeMask = 0x3 << 6,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP HPLL = {
    .conOffset0 = &(TOPCRU->HPLL_CON[0]),
    .conOffset1 = &(TOPCRU->HPLL_CON[1]),
    .conOffset2 = &(TOPCRU->HPLL_CON[2]),
    .conOffset3 = &(TOPCRU->HPLL_CON[3]),
    .modeOffset = &(TOPCRU->MODE_CON00),
    .modeShift = 8,
    .lockShift = 10,
    .modeMask = 0x3 << 8,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP CPLL = {
    .conOffset0 = &(PMU1CRU->CPLL_CON[0]),
    .conOffset1 = &(PMU1CRU->CPLL_CON[1]),
    .conOffset2 = &(PMU1CRU->CPLL_CON[2]),
    .conOffset3 = &(PMU1CRU->CPLL_CON[3]),
    .modeOffset = &(PMU1CRU->PMU1MODE_CON00),
    .modeShift = 0,
    .lockShift = 10,
    .modeMask = 0x3,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP DPLL = {
    .conOffset0 = &(SUBDDRCRU->DPLL_CON[0]),
    .conOffset1 = &(SUBDDRCRU->DPLL_CON[1]),
    .conOffset2 = &(SUBDDRCRU->DPLL_CON[2]),
    .conOffset3 = &(SUBDDRCRU->DPLL_CON[3]),
    .modeOffset = &(SUBDDRCRU->SUBDDRMODE_CON00),
    .modeShift = 0,
    .lockShift = 10,
    .modeMask = 0x3,
    .rateTable = PLL_TABLE,
};

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/
static void CRU_InitPlls(void)
{
    s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);

    HAL_CRU_DBG("%s: gpll=%ld, cpll=%ld\n", __func__, s_gpllFreq, s_cpllFreq);
}

static uint32_t HAL_CRU_ClkGetUartFreq(eCLOCK_Name clockName)
{
    uint32_t mux = CLK_GET_MUX(clockName);
    uint32_t pRate = 0, rate, n, m;
    uint32_t divSrc, divFrac;
    uint32_t muxSrc;

    switch (clockName) {
    case CLK_PMU1_UART0:
        muxSrc = 0;
        divSrc = CLK_GET_DIV(CLK_PMU1_UART0_SRC);
        divFrac = CLK_GET_DIV(CLK_PMU1_UART0_FRAC);
        pRate = s_cpllFreq / HAL_CRU_ClkGetDiv(divSrc);
        break;

    case CLK_UART1:
        muxSrc = CLK_GET_MUX(CLK_UART1_SRC);
        divSrc = CLK_GET_DIV(CLK_UART1_SRC);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        break;

    case CLK_UART2:
        muxSrc = CLK_GET_MUX(CLK_UART2_SRC);
        divSrc = CLK_GET_DIV(CLK_UART2_SRC);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        break;

    case CLK_UART3:
        muxSrc = CLK_GET_MUX(CLK_UART3_SRC);
        divSrc = CLK_GET_DIV(CLK_UART3_SRC);
        divFrac = CLK_GET_DIV(CLK_UART3_FRAC);
        break;

    case CLK_UART4:
        muxSrc = CLK_GET_MUX(CLK_UART4_SRC);
        divSrc = CLK_GET_DIV(CLK_UART4_SRC);
        divFrac = CLK_GET_DIV(CLK_UART4_FRAC);
        break;

    case CLK_UART5:
        muxSrc = CLK_GET_MUX(CLK_UART5_SRC);
        divSrc = CLK_GET_DIV(CLK_UART5_SRC);
        divFrac = CLK_GET_DIV(CLK_UART5_FRAC);
        break;

    case CLK_UART6:
        muxSrc = CLK_GET_MUX(CLK_UART6_SRC);
        divSrc = CLK_GET_DIV(CLK_UART6_SRC);
        divFrac = CLK_GET_DIV(CLK_UART6_FRAC);
        break;

    case CLK_UART7:
        muxSrc = CLK_GET_MUX(CLK_UART7_SRC);
        divSrc = CLK_GET_DIV(CLK_UART7_SRC);
        divFrac = CLK_GET_DIV(CLK_UART7_FRAC);
        break;

    case CLK_UART8:
        muxSrc = CLK_GET_MUX(CLK_UART8_SRC);
        divSrc = CLK_GET_DIV(CLK_UART8_SRC);
        divFrac = CLK_GET_DIV(CLK_UART8_FRAC);
        break;

    case CLK_UART9:
        muxSrc = CLK_GET_MUX(CLK_UART9_SRC);
        divSrc = CLK_GET_DIV(CLK_UART9_SRC);
        divFrac = CLK_GET_DIV(CLK_UART9_FRAC);
        break;

    default:

        return HAL_INVAL;
    }

    /* attention: UART0 src is fixed at CPLL, 'pRate' is initialized */
    if (pRate == 0) {
        pRate = HAL_CRU_MuxGetFreq2(muxSrc,
                                    s_gpllFreq / HAL_CRU_ClkGetDiv(divSrc),
                                    s_cpllFreq / HAL_CRU_ClkGetDiv(divSrc));
    }

    HAL_CRU_ClkGetFracDiv(divFrac, &n, &m);

    rate = HAL_CRU_MuxGetFreq3(mux, pRate, (pRate / m) * n, PLL_INPUT_OSC_RATE);

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX) => (0x%08lX|0U): rate=%ld, mux=%ld, "
                "prate0=%ld, prate1=%ld, prate2=%d\n",
                __func__, muxSrc, divSrc, mux, rate, HAL_CRU_ClkGetMux(mux),
                pRate, pRate / m * n, PLL_INPUT_OSC_RATE);

    return rate;
}

static HAL_Status HAL_CRU_ClkSetUartFreq(eCLOCK_Name clockName, uint32_t rate)
{
    uint32_t muxSrc, mux = CLK_GET_MUX(clockName);
    uint32_t divSrc, divFrac;
    uint32_t gateId, fracGateId;
    uint32_t n = 0, m = 0, maxDiv;

    switch (clockName) {
    case CLK_PMU1_UART0:
        muxSrc = 0;
        divSrc = CLK_GET_DIV(CLK_PMU1_UART0_SRC);
        divFrac = CLK_GET_DIV(CLK_PMU1_UART0_FRAC);
        gateId = CLK_PMU1_UART0_GATE;
        fracGateId = CLK_PMU1_UART0_FRAC_GATE;
        break;

    case CLK_UART1:
        muxSrc = CLK_GET_MUX(CLK_UART1_SRC);
        divSrc = CLK_GET_DIV(CLK_UART1_SRC);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        gateId = CLK_UART1_GATE;
        fracGateId = CLK_UART1_FRAC_GATE;
        break;

    case CLK_UART2:
        muxSrc = CLK_GET_MUX(CLK_UART2_SRC);
        divSrc = CLK_GET_DIV(CLK_UART2_SRC);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        gateId = CLK_UART2_GATE;
        fracGateId = CLK_UART2_FRAC_GATE;
        break;

    case CLK_UART3:
        muxSrc = CLK_GET_MUX(CLK_UART3_SRC);
        divSrc = CLK_GET_DIV(CLK_UART3_SRC);
        divFrac = CLK_GET_DIV(CLK_UART3_FRAC);
        gateId = CLK_UART3_GATE;
        fracGateId = CLK_UART3_FRAC_GATE;
        break;

    case CLK_UART4:
        muxSrc = CLK_GET_MUX(CLK_UART4_SRC);
        divSrc = CLK_GET_DIV(CLK_UART4_SRC);
        divFrac = CLK_GET_DIV(CLK_UART4_FRAC);
        gateId = CLK_UART4_GATE;
        fracGateId = CLK_UART4_FRAC_GATE;
        break;

    case CLK_UART5:
        muxSrc = CLK_GET_MUX(CLK_UART5_SRC);
        divSrc = CLK_GET_DIV(CLK_UART5_SRC);
        divFrac = CLK_GET_DIV(CLK_UART5_FRAC);
        gateId = CLK_UART5_GATE;
        fracGateId = CLK_UART5_FRAC_GATE;
        break;

    case CLK_UART6:
        muxSrc = CLK_GET_MUX(CLK_UART6_SRC);
        divSrc = CLK_GET_DIV(CLK_UART6_SRC);
        divFrac = CLK_GET_DIV(CLK_UART6_FRAC);
        gateId = CLK_UART6_GATE;
        fracGateId = CLK_UART6_FRAC_GATE;
        break;

    case CLK_UART7:
        muxSrc = CLK_GET_MUX(CLK_UART7_SRC);
        divSrc = CLK_GET_DIV(CLK_UART7_SRC);
        divFrac = CLK_GET_DIV(CLK_UART7_FRAC);
        gateId = CLK_UART7_GATE;
        fracGateId = CLK_UART7_FRAC_GATE;
        break;

    case CLK_UART8:
        muxSrc = CLK_GET_MUX(CLK_UART8_SRC);
        divSrc = CLK_GET_DIV(CLK_UART8_SRC);
        divFrac = CLK_GET_DIV(CLK_UART8_FRAC);
        gateId = CLK_UART8_GATE;
        fracGateId = CLK_UART8_FRAC_GATE;
        break;

    case CLK_UART9:
        muxSrc = CLK_GET_MUX(CLK_UART9_SRC);
        divSrc = CLK_GET_DIV(CLK_UART9_SRC);
        divFrac = CLK_GET_DIV(CLK_UART9_FRAC);
        gateId = CLK_UART9_GATE;
        fracGateId = CLK_UART9_FRAC_GATE;
        break;
    default:

        return HAL_INVAL;
    }

    maxDiv = CLK_DIV_GET_MAXDIV(divSrc);

    HAL_CRU_ClkEnable(gateId);
    HAL_CRU_ClkEnable(fracGateId);

    if (PLL_INPUT_OSC_RATE == rate) {
        HAL_CRU_ClkSetMux(mux, 2);
        HAL_CRU_ClkDisable(fracGateId);
        /* attention: UART0 is fixed at CPLL, 'muxSrc' is set as 0 */
    } else if (DIV_NO_REM(s_cpllFreq, rate, maxDiv) || !muxSrc) {
        HAL_CRU_ClkSetDiv(divSrc, HAL_DIV_ROUND_UP(s_cpllFreq, rate));
        if (muxSrc) {
            HAL_CRU_ClkSetMux(muxSrc, 1);
        }
        HAL_CRU_ClkSetMux(mux, 0);
        HAL_CRU_ClkDisable(fracGateId);
    } else if (DIV_NO_REM(s_gpllFreq, rate, maxDiv)) {
        HAL_CRU_ClkSetDiv(divSrc, s_gpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        HAL_CRU_ClkSetMux(mux, 0);
        HAL_CRU_ClkDisable(fracGateId);
    } else {
        HAL_CRU_FracdivGetConfig(rate, s_gpllFreq, &n, &m);
        HAL_CRU_ClkSetDiv(divSrc, 1);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        HAL_CRU_ClkSetFracDiv(divFrac, n, m);
        HAL_CRU_ClkSetMux(mux, 1);
    }

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX) => (0x%08lX|0U): "
                "rate=%ld, pRate=%ld, muxSrc=%ld, divSrc=%ld, mux=%ld, "
                "maxdiv=%ld, n/m=%ld/%ld\n",
                __func__, muxSrc, divSrc, mux,
                rate, m ? (rate * m / n) : (rate * HAL_CRU_ClkGetDiv(divSrc)),
                HAL_CRU_ClkGetMux(muxSrc), HAL_CRU_ClkGetDiv(divSrc),
                HAL_CRU_ClkGetMux(mux), maxDiv, n, m);

    return HAL_OK;
}

uint32_t HAL_CRU_ClkGetFreq(eCLOCK_Name clockName)
{
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t freq = 0;

    if (!s_gpllFreq || !s_cpllFreq) {
        CRU_InitPlls();
    }

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX)\n", __func__, clkMux, clkDiv);
    HAL_CRU_DBG("%s: gpll=%ld, cpll=%ld\n", __func__, s_gpllFreq, s_cpllFreq);

    switch (clockName) {
    case PLL_APLL:

        return HAL_CRU_GetPllFreq(&APLL);

    case PLL_GPLL:
        freq = HAL_CRU_GetPllFreq(&GPLL);
        s_gpllFreq = freq;

        return freq;

    case PLL_VPLL:

        return HAL_CRU_GetPllFreq(&VPLL);

    case PLL_HPLL:

        return HAL_CRU_GetPllFreq(&HPLL);

    case PLL_CPLL:
        freq = HAL_CRU_GetPllFreq(&CPLL);
        s_cpllFreq = freq;

        return freq;

    case PLL_DPLL:

        return HAL_CRU_GetPllFreq(&DPLL);

    case HCLK_BUS:
    case PCLK_BUS:
        freq = HAL_CRU_MuxGetFreq2(clkMux, s_gpllFreq, s_cpllFreq);

        break;
    case CLK_PMU0_I2C0:
    case CLK_PMU1_SPI0:
    case CLK_PMU1_PWM0:
        freq = HAL_CRU_MuxGetFreq3(clkMux, _MHZ(198), PLL_INPUT_OSC_RATE, PLL_INPUT_32K_RATE);

        break;
    case CLK_I2C:
    case CLK_SPI1:
    case CLK_SPI2:
        freq = HAL_CRU_MuxGetFreq4(clkMux, _MHZ(198), _MHZ(100), _MHZ(50), PLL_INPUT_OSC_RATE);

        break;
    case SCLK_SFC:
        freq = HAL_CRU_MuxGetFreq3(clkMux, s_gpllFreq, s_cpllFreq, PLL_INPUT_OSC_RATE);

        break;
    case CLK_PMU1_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
    case CLK_UART6:
    case CLK_UART7:
    case CLK_UART8:
    case CLK_UART9:

        return HAL_CRU_ClkGetUartFreq(clockName);
    case CLK_PWM1_PERI:
    case CLK_PWM2_PERI:
    case CLK_PWM3_PERI:
        freq = HAL_CRU_MuxGetFreq3(clkMux, _MHZ(100), _MHZ(50), PLL_INPUT_OSC_RATE);

        break;
    default:

        return HAL_INVAL;
    }

    if (!clkMux && !clkDiv) {
        return 0;
    }
    if (clkDiv) {
        freq /= (HAL_CRU_ClkGetDiv(clkDiv));
    }

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX): freq: %ld=%ld/%ld\n",
                __func__, clkMux, clkDiv, freq, freq * HAL_CRU_ClkGetDiv(clkDiv),
                HAL_CRU_ClkGetDiv(clkDiv));

    return freq;
}

HAL_Status HAL_CRU_ClkSetFreq(eCLOCK_Name clockName, uint32_t rate)
{
    HAL_Status error = HAL_OK;
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t mux = 0, div = 1, pRate = 0;

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX): rate=%ld\n", __func__, clkMux, clkDiv, rate);
    HAL_CRU_DBG("%s: gpll=%ld, cpll=%ld\n", __func__, s_gpllFreq, s_cpllFreq);

    if (!s_gpllFreq || !s_cpllFreq) {
        CRU_InitPlls();
    }

    switch (clockName) {
    case PLL_GPLL:
        error = HAL_CRU_SetPllFreq(&GPLL, rate);
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);

        return error;

    case PLL_CPLL:
        error = HAL_CRU_SetPllFreq(&CPLL, rate);
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);

        return error;

    case HCLK_BUS:
    case PCLK_BUS:
        pRate = s_gpllFreq;
        mux = HCLK_BUS_SEL_CLK_GPLL_MUX;

        break;
    case CLK_PMU0_I2C0:
    case CLK_PMU1_SPI0:
    case CLK_PMU1_PWM0:
        mux = HAL_CRU_RoundFreqGetMux3(rate, _MHZ(198), PLL_INPUT_OSC_RATE, PLL_INPUT_32K_RATE, &pRate);

        break;
    case CLK_I2C:
    case CLK_SPI1:
    case CLK_SPI2:
        mux = HAL_CRU_FreqGetMux4(rate, _MHZ(198), _MHZ(100), _MHZ(50), PLL_INPUT_OSC_RATE);

        break;
    case SCLK_SFC:
        mux = HAL_CRU_RoundFreqGetMux3(rate, s_gpllFreq, s_cpllFreq, PLL_INPUT_OSC_RATE, &pRate);

        break;
    case CLK_PMU1_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
    case CLK_UART6:
    case CLK_UART7:
    case CLK_UART8:
    case CLK_UART9:

        return HAL_CRU_ClkSetUartFreq(clockName, rate);
    case CLK_PWM1_PERI:
    case CLK_PWM2_PERI:
    case CLK_PWM3_PERI:
        mux = HAL_CRU_FreqGetMux3(rate, _MHZ(100), _MHZ(50), PLL_INPUT_OSC_RATE);

    default:

        return HAL_INVAL;
    }

    if (!clkMux && !clkDiv) {
        return HAL_INVAL;
    }

    if (pRate) {
        div = HAL_DIV_ROUND_UP(pRate, rate);
    }
    if (clkDiv) {
        div = (div > CLK_DIV_GET_MAXDIV(clkDiv) ?
               CLK_DIV_GET_MAXDIV(clkDiv) : (div));
        HAL_CRU_ClkSetDiv(clkDiv, div);
    }
    if (clkMux) {
        HAL_CRU_ClkSetMux(clkMux, mux);
    }

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX): mux=%ld, rate=%ld, pRate=%ld, div=%ld, maxdiv=%d\n",
                __func__, clkMux, clkDiv, mux, rate, pRate, div, CLK_DIV_GET_MAXDIV(clkDiv));

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
    uint32_t mask = TOPCRU_GLB_RST_CON_WDT_RESET_EXT_EN_MASK | TOPCRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_MASK;
    uint32_t val = 1 << TOPCRU_GLB_RST_CON_WDT_RESET_EXT_EN_SHIFT;

    switch (wdtType) {
    case GLB_RST_FST_WDT0:
        val |= (1 << TOPCRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT);
        break;
    case GLB_RST_SND_WDT0:
        break;
    default:

        return HAL_INVAL;
    }

    TOPCRU->GLB_RST_CON = VAL_MASK_WE(mask, val);

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* SOC_RK3562 && HAL_CRU_MODULE_ENABLED */
