/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(SOC_RK3358) && defined(HAL_CRU_MODULE_ENABLED)

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
    RK_PLL_RATE(1608000000, 1, 67, 1, 1, 1, 0),
    RK_PLL_RATE(1512000000, 1, 126, 2, 1, 1, 0),
    RK_PLL_RATE(1416000000, 1, 118, 2, 1, 1, 0),
    RK_PLL_RATE(1200000000, 1, 100, 2, 1, 1, 0),
    RK_PLL_RATE(1188000000, 1, 99, 2, 1, 1, 0),
    RK_PLL_RATE(1008000000, 1, 84, 2, 1, 1, 0),
    RK_PLL_RATE(1000000000, 3, 250, 2, 1, 1, 0),
    RK_PLL_RATE(816000000, 1, 68, 2, 1, 1, 0),
    RK_PLL_RATE(594000000, 1, 99, 4, 1, 1, 0),
    RK_PLL_RATE(200000000, 1, 100, 3, 4, 1, 0),
    { /* sentinel */ },
};

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

static struct PLL_SETUP CPLL = {
    .conOffset0 = &(CRU->CPLL_CON[0]),
    .conOffset1 = &(CRU->CPLL_CON[1]),
    .conOffset2 = &(CRU->CPLL_CON[2]),
    .conOffset3 = &(CRU->CPLL_CON[3]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 2,
    .lockShift = 10,
    .modeMask = 0x3 << 2,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP GPLL = {
    .conOffset0 = &(CRU->GPLL_CON[0]),
    .conOffset1 = &(CRU->GPLL_CON[1]),
    .conOffset2 = &(CRU->GPLL_CON[2]),
    .conOffset3 = &(CRU->GPLL_CON[3]),
    .modeOffset = &(CRU->PMU_MODE_CON00),
    .modeShift = 0,
    .lockShift = 10,
    .modeMask = 0x3 << 0,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP NPLL = {
    .conOffset0 = &(CRU->NPLL_CON[0]),
    .conOffset1 = &(CRU->NPLL_CON[1]),
    .conOffset2 = &(CRU->NPLL_CON[2]),
    .conOffset3 = &(CRU->NPLL_CON[3]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 6,
    .lockShift = 10,
    .modeMask = 0x3 << 6,
    .rateTable = PLL_TABLE,
};

/********************* Private Variable Definition ***************************/

static uint32_t s_cpllFreq;
static uint32_t s_gpllFreq = 1200 * 1000 * 1000;
static uint32_t s_npllFreq;
static uint32_t usb480m = 480 * 1000 * 1000;
static uint32_t mclk_i2s_in = 0;
static uint32_t clk_gmac_in = 0;

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
    uint32_t divSrc = 0, divFrac = 0;
    uint32_t n, m, pRate = 0;

    switch (clockName) {
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
    case CLK_I2S0_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S0_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S0_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S0_TX_FRAC);
        break;
    case CLK_I2S1:
        muxSrc = CLK_GET_MUX(CLK_I2S1_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S1_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S1_FRAC);
        break;
    case CLK_I2S2:
        muxSrc = CLK_GET_MUX(CLK_I2S2_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S2_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S2_FRAC);
        break;
    case CLK_UART0:
        break;
    default:

        return HAL_INVAL;
    }

    switch (clockName) {
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
        n = (CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0xffff0000) >> 16;
        m = CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0x0000ffff;

        if (HAL_CRU_ClkGetMux(muxSrc) == 0) {
            pRate = s_gpllFreq / HAL_CRU_ClkGetDiv(divSrc);
        } else if (HAL_CRU_ClkGetMux(muxSrc) == 1) {
            pRate = PLL_INPUT_OSC_RATE / HAL_CRU_ClkGetDiv(divSrc);
        } else if (HAL_CRU_ClkGetMux(muxSrc) == 2) {
            pRate = usb480m / HAL_CRU_ClkGetDiv(divSrc);
        } else if (HAL_CRU_ClkGetMux(muxSrc) == 3) {
            pRate = s_npllFreq / HAL_CRU_ClkGetDiv(divSrc);
        }

        if (HAL_CRU_ClkGetMux(mux) == 0) {
            freq = pRate;
        } else if (HAL_CRU_ClkGetMux(mux) == 2) {
            freq = (pRate / m) * n;
        }

        return freq;
    default:

        break;
    }

    if (clockName == CLK_UART0) {
        n = (CRU->PMU_CLKSEL_CON[5] & 0xffff0000) >> 16;
        m = CRU->PMU_CLKSEL_CON[5] & 0x0000ffff;
        muxSrc = (CRU->PMU_CLKSEL_CON[3] & 0xc000) >> 14;
        divSrc = (CRU->PMU_CLKSEL_CON[3] & 0x1f) + 1;
        mux = (CRU->PMU_CLKSEL_CON[4] & 0xc000) >> 14;
        if (muxSrc == 0) {
            pRate = s_gpllFreq / divSrc;
        } else if (muxSrc == 1) {
            pRate = PLL_INPUT_OSC_RATE / divSrc;
        } else if (muxSrc == 2) {
            pRate = usb480m / divSrc;
        } else if (muxSrc == 3) {
            pRate = s_npllFreq / divSrc;
        }

        if (mux == 0) {
            freq = pRate;
        } else if (mux == 2) {
            freq = (pRate / m) * n;
        }

        return freq;
    }

    switch (clockName) {
    case CLK_I2S0_TX:
    case CLK_I2S1:
    case CLK_I2S2:
        n = (CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0xffff0000) >> 16;
        m = CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0x0000ffff;

        if (HAL_CRU_ClkGetMux(muxSrc)) {
            pRate = s_npllFreq / HAL_CRU_ClkGetDiv(divSrc);
        } else {
            pRate = s_gpllFreq / HAL_CRU_ClkGetDiv(divSrc);
        }

        if (HAL_CRU_ClkGetMux(mux) == 0) {
            freq = pRate;
        } else if (HAL_CRU_ClkGetMux(mux) == 1) {
            freq = (pRate / m) * n;
        } else if (HAL_CRU_ClkGetMux(mux) == 2) {
            freq = mclk_i2s_in;
        } else {
            freq = PLL_INPUT_OSC_RATE / 2;
        }

        return freq;
    default:

        break;
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
        break;
    case CLK_UART1:
        muxSrc = CLK_GET_MUX(CLK_UART1_SRC);
        divSrc = CLK_GET_DIV(CLK_UART1_SRC);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        gateId = CLK_UART1_PLL_CLK_GATE;
        fracGateId = CLK_UART1_FRAC_SRC_CLK_GATE;
        break;
    case CLK_UART2:
        muxSrc = CLK_GET_MUX(CLK_UART2_SRC);
        divSrc = CLK_GET_DIV(CLK_UART2_SRC);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        gateId = CLK_UART2_PLL_CLK_GATE;
        fracGateId = CLK_UART2_FRAC_SRC_CLK_GATE;
        break;
    case CLK_UART3:
        muxSrc = CLK_GET_MUX(CLK_UART3_SRC);
        divSrc = CLK_GET_DIV(CLK_UART3_SRC);
        divFrac = CLK_GET_DIV(CLK_UART3_FRAC);
        gateId = CLK_UART3_PLL_CLK_GATE;
        fracGateId = CLK_UART3_FRAC_SRC_CLK_GATE;
        break;
    case CLK_UART4:
        muxSrc = CLK_GET_MUX(CLK_UART4_SRC);
        divSrc = CLK_GET_DIV(CLK_UART4_SRC);
        divFrac = CLK_GET_DIV(CLK_UART4_FRAC);
        gateId = CLK_UART4_PLL_CLK_GATE;
        fracGateId = CLK_UART4_FRAC_SRC_CLK_GATE;
        break;
    case CLK_UART5:
        muxSrc = CLK_GET_MUX(CLK_UART5_SRC);
        divSrc = CLK_GET_DIV(CLK_UART5_SRC);
        divFrac = CLK_GET_DIV(CLK_UART5_FRAC);
        gateId = CLK_UART5_PLL_CLK_GATE;
        fracGateId = CLK_UART5_FRAC_SRC_CLK_GATE;
        break;
    case CLK_I2S0_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S0_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S0_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S0_TX_FRAC);
        gateId = CLK_I2S0_TX_PLL_CLK_GATE;
        fracGateId = CLK_I2S0_TX_FRAC_SRC_CLK_GATE;
        break;
    case CLK_I2S1:
        muxSrc = CLK_GET_MUX(CLK_I2S1_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S1_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S1_FRAC);
        gateId = CLK_I2S1_PLL_CLK_GATE;
        fracGateId = CLK_I2S1_FRAC_SRC_CLK_GATE;
        break;
    case CLK_I2S2:
        muxSrc = CLK_GET_MUX(CLK_I2S2_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S2_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S2_FRAC);
        gateId = CLK_I2S2_PLL_CLK_GATE;
        fracGateId = CLK_I2S2_FRAC_SRC_CLK_GATE;
        break;
    default:

        return HAL_INVAL;
    }

    switch (clockName) {
    case CLK_UART0:
        if (PLL_INPUT_OSC_RATE == rate) {
            CRU->PMU_CLKSEL_CON[3] = 0xffff0000 | (1 << 14);
            CRU->PMU_CLKSEL_CON[4] = 0xffff0000;
        } else if ((!(s_npllFreq % rate)) && ((s_npllFreq / rate) < 128)) {
            divSrc = s_npllFreq / rate;
            CRU->PMU_CLKSEL_CON[3] = 0xffff0000 | (3 << 14) | (divSrc - 1);
            CRU->PMU_CLKSEL_CON[4] = 0xffff0000;
        } else if ((!(usb480m % rate)) && ((usb480m / rate) < 128)) {
            divSrc = usb480m / rate;
            CRU->PMU_CLKSEL_CON[3] = 0xffff0000 | (2 << 14) | (divSrc - 1);
            CRU->PMU_CLKSEL_CON[4] = 0xffff0000;
        } else if ((!(s_gpllFreq % rate)) && ((s_gpllFreq / rate) < 128)) {
            divSrc = s_gpllFreq / rate;
            CRU->PMU_CLKSEL_CON[3] = 0xffff0000 | (0 << 14) | (divSrc - 1);
            CRU->PMU_CLKSEL_CON[4] = 0xffff0000;
        } else {
            HAL_CRU_FracdivGetConfig(rate, s_gpllFreq / 2, &n, &m);
            CRU->PMU_CLKSEL_CON[3] = 0xffff0001;
            CRU->PMU_CLKSEL_CON[5] = (n << 16) | m;
            CRU->PMU_CLKSEL_CON[4] = 0xffff0000 | (2 << 14);
        }
        break;
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
        HAL_CRU_ClkEnable(gateId);
        HAL_CRU_ClkEnable(fracGateId);
        if (PLL_INPUT_OSC_RATE == rate) {
            HAL_CRU_ClkSetMux(muxSrc, 1);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else if ((!(s_npllFreq % rate)) && ((s_npllFreq / rate) < 128)) {
            HAL_CRU_ClkSetDiv(divSrc, s_npllFreq / rate);
            HAL_CRU_ClkSetMux(muxSrc, 3);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else if ((!(usb480m % rate)) && ((usb480m / rate) < 128)) {
            HAL_CRU_ClkSetDiv(divSrc, usb480m / rate);
            HAL_CRU_ClkSetMux(muxSrc, 2);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else if ((!(s_gpllFreq % rate)) && ((s_gpllFreq / rate) < 128)) {
            HAL_CRU_ClkSetDiv(divSrc, s_gpllFreq / rate);
            HAL_CRU_ClkSetMux(muxSrc, 0);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else {
            HAL_CRU_FracdivGetConfig(rate, s_gpllFreq / 2, &n, &m);
            HAL_CRU_ClkSetDiv(divSrc, 2);
            HAL_CRU_ClkSetMux(muxSrc, 0);
            CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;
            HAL_CRU_ClkSetMux(mux, 2);
        }
        break;
    case CLK_I2S0_TX:
    case CLK_I2S1:
    case CLK_I2S2:
        HAL_CRU_ClkEnable(gateId);
        HAL_CRU_ClkEnable(fracGateId);
        if (PLL_INPUT_OSC_RATE / 2 == rate) {
            HAL_CRU_ClkSetMux(muxSrc, 0);
            HAL_CRU_ClkSetMux(mux, 3);
            HAL_CRU_ClkDisable(fracGateId);
        } else if (mclk_i2s_in == rate) {
            HAL_CRU_ClkSetMux(muxSrc, 0);
            HAL_CRU_ClkSetMux(mux, 2);
            HAL_CRU_ClkDisable(fracGateId);
        } else if ((!(s_npllFreq % rate)) && ((s_npllFreq / rate) < 128)) {
            HAL_CRU_ClkSetDiv(divSrc, s_npllFreq / rate);
            HAL_CRU_ClkSetMux(muxSrc, 1);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else if ((!(s_gpllFreq % rate)) && ((s_gpllFreq / rate) < 128)) {
            HAL_CRU_ClkSetDiv(divSrc, s_gpllFreq / rate);
            HAL_CRU_ClkSetMux(muxSrc, 0);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else {
            HAL_CRU_FracdivGetConfig(rate, s_gpllFreq / 2, &n, &m);
            HAL_CRU_ClkSetDiv(divSrc, 2);
            HAL_CRU_ClkSetMux(muxSrc, 0);
            CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;
            HAL_CRU_ClkSetMux(mux, 1);
        }
        break;
    default:
        break;
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
    uint32_t pRate = 0, freq = 0;

    if (!s_cpllFreq) {
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);
    }
    if (!s_gpllFreq) {
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    }
    if (!s_npllFreq) {
        s_npllFreq = HAL_CRU_GetPllFreq(&NPLL);
    }

    switch (clockName) {
    case PLL_APLL:
        freq = HAL_CRU_GetPllFreq(&APLL);

        return freq;
    case PLL_CPLL:
        freq = HAL_CRU_GetPllFreq(&CPLL);
        s_cpllFreq = freq;

        return freq;
    case PLL_NPLL:
        freq = HAL_CRU_GetPllFreq(&NPLL);
        s_npllFreq = freq;

        return freq;
    case PLL_GPLL:
        freq = HAL_CRU_GetPllFreq(&GPLL);
        s_gpllFreq = freq;

        return freq;

    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
    case CLK_UART0:
    case CLK_I2S1:
    case CLK_I2S0_TX:
    case CLK_I2S2:
        freq = HAL_CRU_ClkFracGetFreq(clockName);

        return freq;
    case MCLK_I2S1:
        if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = PLL_INPUT_OSC_RATE / 2;
        } else {
            freq = HAL_CRU_ClkFracGetFreq(CLK_I2S1);
        }

        return freq;
    case MCLK_I2S2:
        if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = PLL_INPUT_OSC_RATE / 2;
        } else {
            freq = HAL_CRU_ClkFracGetFreq(CLK_I2S2);
        }

        return freq;
    case MCLK_I2S0_TX:
        if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = PLL_INPUT_OSC_RATE / 2;
        } else {
            freq = HAL_CRU_ClkFracGetFreq(CLK_I2S0_TX);
        }

        return freq;
    case CLK_SDMMC:
    case CLK_SDIO:
    case CLK_EMMC:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            pRate = s_gpllFreq;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            pRate = s_cpllFreq;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            pRate = s_npllFreq;
        } else {
            pRate = PLL_INPUT_OSC_RATE;
        }

        break;

    case CLK_MAC_OUT:
    case CLK_NANDC:
    case CLK_GMAC:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            pRate = s_gpllFreq;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            pRate = s_cpllFreq;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            pRate = s_npllFreq;
        }

        break;
    case ACLK_PERI:
    case HCLK_PERI:
    case ACLK_BUS:
    case HCLK_BUS:
    case CLK_SFC:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            pRate = s_gpllFreq;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            pRate = s_cpllFreq;
        }

        break;

    case PCLK_BUS:
        pRate = HAL_CRU_ClkGetFreq(ACLK_BUS);

        break;

    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_I2C2:
    case CLK_I2C3:
    case CLK_PWM0:
    case CLK_PWM1:
    case CLK_SPI0:
    case CLK_SPI1:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            pRate = s_gpllFreq;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            pRate = PLL_INPUT_OSC_RATE;
        }

        break;
    case CLK_TSADC:
    case CLK_SARADC:
    case CLK_OTP:
        pRate = PLL_INPUT_OSC_RATE;

        break;
    case CLK_RMII:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 10 * 1000 * 1000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 100 * 1000 * 1000;
        }

        return freq;
    case CLK_RMII_EXTSRC:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = HAL_CRU_ClkGetFreq(CLK_GMAC);
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = clk_gmac_in;
        }

        return freq;
    case PCLK_GMAC:
        pRate = HAL_CRU_ClkGetFreq(ACLK_PERI);
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
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t mux = 0, div = 0, pRate = 0;

    if (!s_cpllFreq) {
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);
    }
    if (!s_gpllFreq) {
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    }
    if (!s_npllFreq) {
        s_npllFreq = HAL_CRU_GetPllFreq(&NPLL);
    }

    switch (clockName) {
    case PLL_APLL:
        error = HAL_CRU_SetPllFreq(&APLL, rate);

        return error;
    case PLL_CPLL:
        error = HAL_CRU_SetPllFreq(&CPLL, rate);
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);

        return error;
    case PLL_NPLL:

        error = HAL_CRU_SetPllFreq(&NPLL, rate);
        s_npllFreq = HAL_CRU_GetPllFreq(&NPLL);

        return error;
    case PLL_GPLL:

        error = HAL_CRU_SetPllFreq(&GPLL, rate);
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);

        return error;

    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
    case CLK_UART0:
    case CLK_I2S1:
    case CLK_I2S0_TX:
    case CLK_I2S2:
        error = HAL_CRU_ClkFracSetFreq(clockName, rate);

        return error;
    case MCLK_I2S1:
        if (PLL_INPUT_OSC_RATE / 2 == rate) {
            mux = 1;
        } else {
            HAL_CRU_ClkFracSetFreq(CLK_I2S1, rate);
            mux = 0;
        }
        break;
    case MCLK_I2S2:
        if (PLL_INPUT_OSC_RATE / 2 == rate) {
            mux = 1;
        } else {
            HAL_CRU_ClkFracSetFreq(CLK_I2S2, rate);
            mux = 0;
        }
        break;
    case MCLK_I2S0_TX:
        if (PLL_INPUT_OSC_RATE / 2 == rate) {
            mux = 1;
        } else {
            HAL_CRU_ClkFracSetFreq(CLK_I2S0_TX, rate);
            mux = 0;
        }
        break;
    case CLK_SDMMC:
    case CLK_SDIO:
    case CLK_EMMC:
        if (PLL_INPUT_OSC_RATE % rate == 0) {
            pRate = PLL_INPUT_OSC_RATE;
            mux = 3;
        } else if (s_npllFreq % rate == 0) {
            pRate = s_npllFreq;
            mux = 2;
        } else if (s_cpllFreq % rate == 0) {
            pRate = s_cpllFreq;
            mux = 1;
        } else {
            pRate = s_gpllFreq;
            mux = 0;
        }
        break;

    case CLK_MAC_OUT:
    case CLK_NANDC:
    case CLK_GMAC:
        if (s_npllFreq % rate == 0) {
            pRate = s_npllFreq;
            mux = 2;
        } else if (s_cpllFreq % rate == 0) {
            pRate = s_cpllFreq;
            mux = 1;
        } else {
            pRate = s_gpllFreq;
            mux = 0;
        }
        break;
    case ACLK_PERI:
    case HCLK_PERI:
    case ACLK_BUS:
    case HCLK_BUS:
    case CLK_SFC:
        if (s_cpllFreq % rate == 0) {
            pRate = s_cpllFreq;
            mux = 1;
        } else {
            pRate = s_gpllFreq;
            mux = 0;
        }
        break;

    case PCLK_BUS:
        pRate = HAL_CRU_ClkGetFreq(ACLK_BUS);

        break;
    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_I2C2:
    case CLK_I2C3:
    case CLK_PWM0:
    case CLK_PWM1:
    case CLK_SPI0:
    case CLK_SPI1:
        if (PLL_INPUT_OSC_RATE % rate == 0) {
            pRate = PLL_INPUT_OSC_RATE;
            mux = 1;
        } else {
            pRate = s_gpllFreq;
            mux = 0;
        }
        break;
    case CLK_TSADC:
    case CLK_SARADC:
    case CLK_OTP:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case CLK_RMII:
        if (rate == 10 * 1000 * 1000) {
            mux = 0;
        } else {
            mux = 1;
        }
        break;
    case CLK_RMII_EXTSRC:
        if (rate == clk_gmac_in) {
            mux = 1;
        } else {
            mux = 0;
        }
        break;
    case PCLK_GMAC:
        pRate = HAL_CRU_ClkGetFreq(ACLK_PERI);
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
    uint32_t mask = CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_MASK;
    uint32_t val = 0;

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

#endif /* SOC_RK3358 && HAL_CRU_MODULE_ENABLED */
