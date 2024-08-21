/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(SOC_RK3308) && defined(HAL_CRU_MODULE_ENABLED)

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
    RK_PLL_RATE(1300000000, 3, 325, 2, 1, 1, 0),
    RK_PLL_RATE(1200000000, 1, 100, 2, 1, 1, 0),
    RK_PLL_RATE(1179648000, 4, 393, 2, 1, 0, 3623878),
    RK_PLL_RATE(1188000000, 1, 99, 2, 1, 1, 0),
    RK_PLL_RATE(1008000000, 1, 84, 2, 1, 1, 0),
    RK_PLL_RATE(1000000000, 3, 250, 2, 1, 1, 0),
    RK_PLL_RATE(903168000, 2, 301, 4, 1, 0, 939524),
    RK_PLL_RATE(816000000, 1, 68, 2, 1, 1, 0),
    RK_PLL_RATE(600000000, 1, 100, 4, 1, 1, 0),
    { /* sentinel */ },
};

static uint32_t s_apllFreq;
static uint32_t s_dpllFreq;
static uint32_t s_vpll0Freq;
static uint32_t s_vpll1Freq;

static struct PLL_SETUP APLL = {
    .conOffset0 = &(CRU->APLL_CON[0]),
    .conOffset1 = &(CRU->APLL_CON[1]),
    .conOffset2 = &(CRU->APLL_CON[2]),
    .conOffset3 = &(CRU->APLL_CON[3]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 0,
    .lockShift = 10,
    .modeMask = 0x3,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP DPLL = {
    .conOffset0 = &(CRU->DPLL_CON[0]),
    .conOffset1 = &(CRU->DPLL_CON[1]),
    .conOffset2 = &(CRU->DPLL_CON[2]),
    .conOffset3 = &(CRU->DPLL_CON[3]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 2,
    .lockShift = 10,
    .modeMask = 0x3 << 2,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP VPLL0 = {
    .conOffset0 = &(CRU->VPLL0_CON[0]),
    .conOffset1 = &(CRU->VPLL0_CON[1]),
    .conOffset2 = &(CRU->VPLL0_CON[2]),
    .conOffset3 = &(CRU->VPLL0_CON[3]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 4,
    .lockShift = 10,
    .modeMask = 0x3 << 4,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP VPLL1 = {
    .conOffset0 = &(CRU->VPLL1_CON[0]),
    .conOffset1 = &(CRU->VPLL1_CON[1]),
    .conOffset2 = &(CRU->VPLL1_CON[2]),
    .conOffset3 = &(CRU->VPLL1_CON[3]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 6,
    .lockShift = 10,
    .modeMask = 0x3 << 6,
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
    case CLK_PDM:
        muxSrc = CLK_GET_MUX(CLK_PDM_SRC);
        divSrc = CLK_GET_DIV(CLK_PDM_SRC);
        divFrac = CLK_GET_DIV(CLK_PDM_FRAC);
        break;
    case CLK_SPDIFTX:
        muxSrc = CLK_GET_MUX(CLK_SPDIFTX_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIFTX_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIFTX_FRAC);
        break;
    case CLK_SPDIFRX:
        muxSrc = CLK_GET_MUX(CLK_SPDIFRX_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIFRX_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIFRX_FRAC);
        break;
    case CLK_I2S0_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S0_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S0_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S0_8CH_TX_FRAC);
        break;
    case CLK_I2S0_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S0_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S0_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S0_8CH_RX_FRAC);
        break;
    case CLK_I2S1_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S1_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S1_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S1_8CH_TX_FRAC);
        break;
    case CLK_I2S1_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S1_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S1_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S1_8CH_RX_FRAC);
        break;
    case CLK_I2S2_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S2_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S2_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S2_8CH_TX_FRAC);
        break;
    case CLK_I2S2_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S2_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S2_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S2_8CH_RX_FRAC);
        break;
    case CLK_I2S3_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S3_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S3_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S3_8CH_TX_FRAC);
        break;
    case CLK_I2S3_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S3_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S3_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S3_8CH_RX_FRAC);
        break;
    case CLK_I2S0_2CH:
        muxSrc = CLK_GET_MUX(CLK_I2S0_2CH_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S0_2CH_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S0_2CH_FRAC);
        break;
    case CLK_I2S1_2CH:
        muxSrc = CLK_GET_MUX(CLK_I2S1_2CH_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S1_2CH_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S1_2CH_FRAC);
        break;

    default:

        return HAL_INVAL;
    }

    n = (CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0xffff0000) >> 16;
    m = CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0x0000ffff;

    switch (clockName) {
    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
        if (HAL_CRU_ClkGetMux(muxSrc) == 0) {
            pRate = s_dpllFreq / HAL_CRU_ClkGetDiv(divSrc);
        } else if (HAL_CRU_ClkGetMux(muxSrc) == 1) {
            pRate = s_vpll0Freq / HAL_CRU_ClkGetDiv(divSrc);
        } else if (HAL_CRU_ClkGetMux(muxSrc) == 2) {
            pRate = s_vpll1Freq / HAL_CRU_ClkGetDiv(divSrc);
        } else {
            pRate = PLL_INPUT_OSC_RATE / HAL_CRU_ClkGetDiv(divSrc);
        }

        if (HAL_CRU_ClkGetMux(mux) == 0) {
            freq = pRate;
        } else if (HAL_CRU_ClkGetMux(mux) == 2) {
            freq = (pRate / m) * n;
        }

        return freq;
    case CLK_PDM:
    case CLK_SPDIFTX:
    case CLK_SPDIFRX:
    case CLK_I2S0_8CH_TX:
    case CLK_I2S0_8CH_RX:
    case CLK_I2S1_8CH_TX:
    case CLK_I2S1_8CH_RX:
    case CLK_I2S2_8CH_TX:
    case CLK_I2S2_8CH_RX:
    case CLK_I2S3_8CH_TX:
    case CLK_I2S3_8CH_RX:
    case CLK_I2S0_2CH:
    case CLK_I2S1_2CH:
        if (HAL_CRU_ClkGetMux(muxSrc) == 0) {
            pRate = s_vpll0Freq / HAL_CRU_ClkGetDiv(divSrc);
        } else if (HAL_CRU_ClkGetMux(muxSrc) == 1) {
            pRate = s_vpll1Freq / HAL_CRU_ClkGetDiv(divSrc);
        } else {
            pRate = PLL_INPUT_OSC_RATE / HAL_CRU_ClkGetDiv(divSrc);
        }

        if (HAL_CRU_ClkGetMux(mux) == 0) {
            freq = pRate;
        } else if (HAL_CRU_ClkGetMux(mux) == 1) {
            freq = (pRate / m) * n;
        }

        return freq;
    default:

        return HAL_INVAL;
    }
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
    uint32_t fracGateId;

    switch (clockName) {
    case CLK_UART0:
        muxSrc = CLK_GET_MUX(CLK_UART0_SRC);
        divSrc = CLK_GET_DIV(CLK_UART0_SRC);
        divFrac = CLK_GET_DIV(CLK_UART0_FRAC);
        fracGateId = CLK_UART0_DIVFRAC_CLK_GATE;
        break;
    case CLK_UART1:
        muxSrc = CLK_GET_MUX(CLK_UART1_SRC);
        divSrc = CLK_GET_DIV(CLK_UART1_SRC);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        fracGateId = CLK_UART1_DIVFRAC_CLK_GATE;
        break;
    case CLK_UART2:
        muxSrc = CLK_GET_MUX(CLK_UART2_SRC);
        divSrc = CLK_GET_DIV(CLK_UART2_SRC);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        fracGateId = CLK_UART2_DIVFRAC_CLK_GATE;
        break;
    case CLK_UART3:
        muxSrc = CLK_GET_MUX(CLK_UART3_SRC);
        divSrc = CLK_GET_DIV(CLK_UART3_SRC);
        divFrac = CLK_GET_DIV(CLK_UART3_FRAC);
        fracGateId = CLK_UART3_DIVFRAC_CLK_GATE;
        break;
    case CLK_UART4:
        muxSrc = CLK_GET_MUX(CLK_UART4_SRC);
        divSrc = CLK_GET_DIV(CLK_UART4_SRC);
        divFrac = CLK_GET_DIV(CLK_UART4_FRAC);
        fracGateId = CLK_UART4_DIVFRAC_CLK_GATE;
        break;
    case CLK_PDM:
        muxSrc = CLK_GET_MUX(CLK_PDM_SRC);
        divSrc = CLK_GET_DIV(CLK_PDM_SRC);
        divFrac = CLK_GET_DIV(CLK_PDM_FRAC);
        fracGateId = CLK_PDM_DIVFRAC_CLK_GATE;

        break;
    case CLK_SPDIFTX:
        muxSrc = CLK_GET_MUX(CLK_SPDIFTX_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIFTX_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIFTX_FRAC);
        fracGateId = CLK_SPDIFTX_DIVFRAC_CLK_GATE;

        break;
    case CLK_SPDIFRX:
        muxSrc = CLK_GET_MUX(CLK_SPDIFRX_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIFRX_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIFRX_FRAC);
        fracGateId = CLK_SPDIFRX_DIVFRAC_CLK_GATE;

        break;
    case CLK_I2S0_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S0_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S0_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S0_8CH_TX_FRAC);
        fracGateId = CLK_I2S0_8CH_TX_DIVFRAC_CLK_GATE;
        break;
    case CLK_I2S0_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S0_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S0_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S0_8CH_RX_FRAC);
        fracGateId = CLK_I2S0_8CH_RX_DIVFRAC_CLK_GATE;
        break;
    case CLK_I2S1_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S1_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S1_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S1_8CH_TX_FRAC);
        fracGateId = CLK_I2S1_8CH_TX_DIVFRAC_CLK_GATE;
        break;
    case CLK_I2S1_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S1_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S1_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S1_8CH_RX_FRAC);
        fracGateId = CLK_I2S1_8CH_RX_DIVFRAC_CLK_GATE;
        break;
    case CLK_I2S2_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S2_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S2_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S2_8CH_TX_FRAC);
        fracGateId = CLK_I2S2_8CH_TX_DIVFRAC_CLK_GATE;
        break;
    case CLK_I2S2_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S2_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S2_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S2_8CH_RX_FRAC);
        fracGateId = CLK_I2S2_8CH_RX_DIVFRAC_CLK_GATE;
        break;
    case CLK_I2S3_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S3_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S3_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S3_8CH_TX_FRAC);
        fracGateId = CLK_I2S3_8CH_TX_DIVFRAC_CLK_GATE;
        break;
    case CLK_I2S3_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S3_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S3_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S3_8CH_RX_FRAC);
        fracGateId = CLK_I2S3_8CH_RX_DIVFRAC_CLK_GATE;
        break;
    case CLK_I2S0_2CH:
        muxSrc = CLK_GET_MUX(CLK_I2S0_2CH_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S0_2CH_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S0_2CH_FRAC);
        fracGateId = CLK_I2S0_2CH_DIVFRAC_CLK_GATE;
        break;
    case CLK_I2S1_2CH:
        muxSrc = CLK_GET_MUX(CLK_I2S1_2CH_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S1_2CH_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S1_2CH_FRAC);
        fracGateId = CLK_I2S1_2CH_DIVFRAC_CLK_GATE;
        break;

    default:

        return HAL_INVAL;
    }
    switch (clockName) {
    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
        HAL_CRU_ClkEnable(fracGateId);

        if ((!(PLL_INPUT_OSC_RATE % rate))) {
            HAL_CRU_ClkSetDiv(divSrc, 1);
            HAL_CRU_ClkSetMux(muxSrc, 4);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else if ((!(s_dpllFreq % rate)) && ((s_dpllFreq / rate) < 31)) {
            HAL_CRU_ClkSetDiv(divSrc, s_dpllFreq / rate);
            HAL_CRU_ClkSetMux(muxSrc, 0);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else if ((!(s_vpll0Freq % rate)) && ((s_vpll0Freq / rate) < 31)) {
            HAL_CRU_ClkSetDiv(divSrc, s_vpll0Freq / rate);
            HAL_CRU_ClkSetMux(muxSrc, 1);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else if ((!(s_vpll1Freq % rate)) && ((s_vpll1Freq / rate) < 31)) {
            HAL_CRU_ClkSetDiv(divSrc, s_vpll1Freq / rate);
            HAL_CRU_ClkSetMux(muxSrc, 2);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else {
            HAL_CRU_FracdivGetConfig(rate, s_dpllFreq, &n, &m);
            HAL_CRU_ClkSetDiv(divSrc, 1);
            HAL_CRU_ClkSetMux(muxSrc, 0);
            CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;
            HAL_CRU_ClkSetMux(mux, 2);
        }
        break;
    case CLK_PDM:
    case CLK_SPDIFTX:
    case CLK_SPDIFRX:
    case CLK_I2S0_8CH_TX:
    case CLK_I2S0_8CH_RX:
    case CLK_I2S1_8CH_TX:
    case CLK_I2S1_8CH_RX:
    case CLK_I2S2_8CH_TX:
    case CLK_I2S2_8CH_RX:
    case CLK_I2S3_8CH_TX:
    case CLK_I2S3_8CH_RX:
    case CLK_I2S0_2CH:
    case CLK_I2S1_2CH:
        HAL_CRU_ClkEnable(fracGateId);

        if ((!(PLL_INPUT_OSC_RATE % rate))) {
            HAL_CRU_ClkSetMux(mux, 2);
            HAL_CRU_ClkDisable(fracGateId);
        } else if ((!(s_vpll0Freq % rate)) && ((s_vpll0Freq / rate) < 31)) {
            HAL_CRU_ClkSetDiv(divSrc, s_vpll0Freq / rate);
            HAL_CRU_ClkSetMux(muxSrc, 0);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else if ((!(s_vpll1Freq % rate)) && ((s_vpll1Freq / rate) < 31)) {
            HAL_CRU_ClkSetDiv(divSrc, s_vpll1Freq / rate);
            HAL_CRU_ClkSetMux(muxSrc, 1);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else {
            HAL_CRU_FracdivGetConfig(rate, s_vpll0Freq, &n, &m);
            HAL_CRU_ClkSetDiv(divSrc, 1);
            HAL_CRU_ClkSetMux(muxSrc, 0);
            CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;
            HAL_CRU_ClkSetMux(mux, 1);
        }
        break;
    default:

        return HAL_INVAL;
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
    uint32_t pRate = 0, freq;

    if (!s_dpllFreq) {
        s_dpllFreq = HAL_CRU_GetPllFreq(&DPLL);
    }
    if (!s_vpll0Freq) {
        s_vpll0Freq = HAL_CRU_GetPllFreq(&VPLL0);
    }
    if (!s_vpll1Freq) {
        s_vpll1Freq = HAL_CRU_GetPllFreq(&VPLL1);
    }

    switch (clockName) {
    case PLL_APLL:
        freq = HAL_CRU_GetPllFreq(&APLL);
        s_apllFreq = freq;

        return freq;
    case PLL_DPLL:
        freq = HAL_CRU_GetPllFreq(&DPLL);
        s_dpllFreq = freq;

        return freq;
    case PLL_VPLL0:
        freq = HAL_CRU_GetPllFreq(&VPLL0);
        s_vpll0Freq = freq;

        return freq;
    case PLL_VPLL1:
        freq = HAL_CRU_GetPllFreq(&VPLL1);
        s_vpll1Freq = freq;

        return freq;
    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_PDM:
    case CLK_SPDIFTX:
    case CLK_SPDIFRX:
    case CLK_I2S0_8CH_TX:
    case CLK_I2S0_8CH_RX:
    case CLK_I2S1_8CH_TX:
    case CLK_I2S1_8CH_RX:
    case CLK_I2S2_8CH_TX:
    case CLK_I2S2_8CH_RX:
    case CLK_I2S3_8CH_TX:
    case CLK_I2S3_8CH_RX:
    case CLK_I2S0_2CH:
    case CLK_I2S1_2CH:
        freq = HAL_CRU_ClkFracGetFreq(clockName);

        return freq;
    case CLK_TSADC:
    case CLK_SARADC:
    case CLK_OTP:
    case CLK_OTP_USR:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_I2C2:
    case CLK_I2C3:
    case CLK_PWM:
    case CLK_PWM1:
    case CLK_PWM2:
    case CLK_SPI0:
    case CLK_SPI1:
    case CLK_SPI2:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            pRate = s_dpllFreq;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            pRate = s_vpll0Freq;
        } else {
            pRate = PLL_INPUT_OSC_RATE;
        }

        break;
    default:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            pRate = s_dpllFreq;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            pRate = s_vpll0Freq;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            pRate = s_vpll1Freq;
        }
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

    if (!s_dpllFreq) {
        s_dpllFreq = HAL_CRU_GetPllFreq(&DPLL);
    }
    if (!s_vpll0Freq) {
        s_vpll0Freq = HAL_CRU_GetPllFreq(&VPLL0);
    }
    if (!s_vpll1Freq) {
        s_vpll1Freq = HAL_CRU_GetPllFreq(&VPLL1);
    }

    switch (clockName) {
    case PLL_APLL:
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
    case PLL_DPLL:
        error = HAL_CRU_SetPllFreq(&DPLL, rate);
        s_dpllFreq = HAL_CRU_GetPllFreq(&DPLL);

        return error;
    case PLL_VPLL0:
        error = HAL_CRU_SetPllFreq(&VPLL0, rate);
        s_vpll0Freq = HAL_CRU_GetPllFreq(&VPLL0);

        return error;
    case PLL_VPLL1:
        error = HAL_CRU_SetPllFreq(&VPLL1, rate);
        s_vpll1Freq = HAL_CRU_GetPllFreq(&VPLL1);

        return error;
    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_PDM:
    case CLK_SPDIFTX:
    case CLK_SPDIFRX:
    case CLK_I2S0_8CH_TX:
    case CLK_I2S0_8CH_RX:
    case CLK_I2S1_8CH_TX:
    case CLK_I2S1_8CH_RX:
    case CLK_I2S2_8CH_TX:
    case CLK_I2S2_8CH_RX:
    case CLK_I2S3_8CH_TX:
    case CLK_I2S3_8CH_RX:
    case CLK_I2S0_2CH:
    case CLK_I2S1_2CH:
        error = HAL_CRU_ClkFracSetFreq(clockName, rate);

        return error;
    case CLK_TSADC:
    case CLK_SARADC:
    case CLK_OTP:
    case CLK_OTP_USR:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_I2C2:
    case CLK_I2C3:
    case CLK_PWM:
    case CLK_PWM1:
    case CLK_PWM2:
    case CLK_SPI0:
    case CLK_SPI1:
    case CLK_SPI2:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            pRate = s_dpllFreq;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            pRate = s_vpll0Freq;
        } else {
            pRate = PLL_INPUT_OSC_RATE;
        }

        break;
    default:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            pRate = s_dpllFreq;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            pRate = s_vpll0Freq;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            pRate = s_vpll1Freq;
        }
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
    uint32_t mask = CRU_GLB_RST_CON_WDT_RESET_EXT_EN_MASK | CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_MASK;
    uint32_t val = 1 << CRU_GLB_RST_CON_WDT_RESET_EXT_EN_SHIFT;

    switch (wdtType) {
    case GLB_RST_FST_WDT0:
        val |= (1 << CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT);
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

#endif /* SOC_RK3308 && HAL_CRU_MODULE_ENABLED */
