/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(RKMCU_RK2206) && defined(HAL_CRU_MODULE_ENABLED)

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
    RK_PLL_RATE(491520000, 1, 24, 2, 1, 0, 9663676),
    RK_PLL_RATE(451584000, 1, 22, 2, 1, 0, 9717363),
    RK_PLL_RATE(384000000, 5, 48, 1, 1, 1, 0),
    RK_PLL_RATE(297000000, 8, 297, 5, 1, 1, 0),
    RK_PLL_RATE(288000000, 5, 72, 2, 1, 1, 0),
    { /* sentinel */ },
};

static uint32_t s_gpllFreq;
static uint32_t s_vpllFreq;

static struct PLL_SETUP GPLL = {
    .conOffset0 = &(CRU->GPLL_CON[0]),
    .conOffset1 = &(CRU->GPLL_CON[1]),
    .conOffset2 = &(CRU->GPLL_CON[2]),
    .conOffset3 = &(CRU->GPLL_CON[3]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 0,
    .lockShift = 10,
    .modeMask = 0x3 << 0,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP VPLL = {
    .conOffset0 = &(CRU->VPLL_CON[0]),
    .conOffset1 = &(CRU->VPLL_CON[1]),
    .conOffset2 = &(CRU->VPLL_CON[2]),
    .conOffset3 = &(CRU->VPLL_CON[3]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 4,
    .lockShift = 10,
    .modeMask = 0x3 << 4,
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
    case MCLK_I2S8CH_0_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S8CH_0_TX_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S8CH_0_TX_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S8CH_0_TX_FRAC);
        break;
    case MCLK_I2S8CH_0_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S8CH_0_RX_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S8CH_0_RX_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S8CH_0_RX_FRAC);
        break;
    case MCLK_I2S8CH_1_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S8CH_1_TX_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S8CH_1_TX_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S8CH_1_TX_FRAC);
        break;
    case MCLK_I2S8CH_1_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S8CH_1_RX_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S8CH_1_RX_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S8CH_1_RX_FRAC);
        break;
    case CLK_AUDIOPWM:
        muxSrc = CLK_GET_MUX(CLK_AUDIOPWM_PLL);
        divSrc = CLK_GET_DIV(CLK_AUDIOPWM_PLL);
        divFrac = CLK_GET_DIV(CLK_AUDIOPWM_FRAC);
        break;
    case CLK_32K:
        divFrac = CLK_GET_DIV(CLK_32K_FRAC);
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
        if (HAL_CRU_ClkGetMux(muxSrc)) {
            pRate = PLL_INPUT_OSC_RATE;
        } else {
            pRate = s_gpllFreq;
        }
        break;
    case MCLK_I2S8CH_0_TX:
    case MCLK_I2S8CH_0_RX:
    case MCLK_I2S8CH_1_TX:
    case MCLK_I2S8CH_1_RX:
        if (HAL_CRU_ClkGetMux(muxSrc) == 0) {
            pRate = s_gpllFreq;
        } else if (HAL_CRU_ClkGetMux(muxSrc) == 1) {
            pRate = s_vpllFreq;
        } else if (HAL_CRU_ClkGetMux(muxSrc) == 2) {
            pRate = PLL_INPUT_OSC_RATE * 2;
        } else {
            pRate = PLL_INPUT_OSC_RATE;
        }
        break;
    case CLK_AUDIOPWM:
        if (HAL_CRU_ClkGetMux(muxSrc) == 0) {
            pRate = s_gpllFreq;
        } else if (HAL_CRU_ClkGetMux(muxSrc) == 1) {
            pRate = s_vpllFreq;
        } else if (HAL_CRU_ClkGetMux(muxSrc) == 2) {
            pRate = 480000000;
        } else {
            pRate = PLL_INPUT_OSC_RATE;
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
    } else if (HAL_CRU_ClkGetMux(mux) == 1) {
        freq = (pRate / m) * n;
    } else if (HAL_CRU_ClkGetMux(mux) == 2) {
        freq = PLL_INPUT_OSC_RATE / 2;
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
        muxSrc = CLK_GET_MUX(CLK_UART0_PLL);
        divSrc = CLK_GET_DIV(CLK_UART0_PLL);
        divFrac = CLK_GET_DIV(CLK_UART0_FRAC);
        gateId = CLK_UART0_PLL_GATE;
        fracGateId = CLK_UART0_FRAC_GATE;
        break;
    case CLK_UART1:
        muxSrc = CLK_GET_MUX(CLK_UART1_PLL);
        divSrc = CLK_GET_DIV(CLK_UART1_PLL);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        gateId = CLK_UART1_GATE;
        fracGateId = CLK_UART1_FRAC_GATE;
        break;
    case CLK_UART2:
        muxSrc = CLK_GET_MUX(CLK_UART2_PLL);
        divSrc = CLK_GET_DIV(CLK_UART2_PLL);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        gateId = CLK_UART2_PLL_GATE;
        fracGateId = CLK_UART2_FRAC_GATE;
        break;
    case MCLK_I2S8CH_0_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S8CH_0_TX_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S8CH_0_TX_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S8CH_0_TX_FRAC);
        gateId = CLK_I2S8CH_0_TX_PLL_GATE;
        fracGateId = CLK_I2S8CH_0_TX_FRAC_DIV_GATE;
        break;
    case MCLK_I2S8CH_0_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S8CH_0_RX_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S8CH_0_RX_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S8CH_0_RX_FRAC);
        gateId = CLK_I2S8CH_0_RX_PLL_GATE;
        fracGateId = CLK_I2S8CH_0_RX_FRAC_DIV_GATE;
        break;
    case MCLK_I2S8CH_1_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S8CH_1_TX_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S8CH_1_TX_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S8CH_1_TX_FRAC);
        gateId = CLK_I2S8CH_1_TX_PLL_GATE;
        fracGateId = CLK_I2S8CH_1_TX_FRAC_DIV_GATE;
        break;
    case MCLK_I2S8CH_1_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S8CH_1_RX_PLL);
        divSrc = CLK_GET_DIV(CLK_I2S8CH_1_RX_PLL);
        divFrac = CLK_GET_DIV(CLK_I2S8CH_1_RX_FRAC);
        gateId = CLK_I2S8CH_1_RX_PLL_GATE;
        fracGateId = CLK_I2S8CH_1_RX_FRAC_DIV_GATE;
        break;
    case CLK_AUDIOPWM:
        muxSrc = CLK_GET_MUX(CLK_AUDIOPWM_PLL);
        divSrc = CLK_GET_DIV(CLK_AUDIOPWM_PLL);
        divFrac = CLK_GET_DIV(CLK_AUDIOPWM_FRAC);
        gateId = CLK_AUDIOPWM_PLL_GATE;
        fracGateId = CLK_AUDIOPWM_FRAC_GATE;
        break;
    case CLK_32K:
        divFrac = CLK_GET_DIV(CLK_32K_FRAC);
        break;
    default:

        return HAL_INVAL;
    }

    switch (clockName) {
    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
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
    case MCLK_I2S8CH_0_TX:
    case MCLK_I2S8CH_0_RX:
    case MCLK_I2S8CH_1_TX:
    case MCLK_I2S8CH_1_RX:
    case CLK_AUDIOPWM:
        HAL_CRU_ClkEnable(gateId);
        HAL_CRU_ClkEnable(fracGateId);
        if ((!(PLL_INPUT_OSC_RATE % rate)) && ((PLL_INPUT_OSC_RATE / rate) < 31)) {
            HAL_CRU_ClkSetDiv(divSrc, PLL_INPUT_OSC_RATE / rate);
            HAL_CRU_ClkSetMux(muxSrc, 3);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else if ((!(s_gpllFreq % rate)) && ((s_gpllFreq / rate) < 31)) {
            HAL_CRU_ClkSetDiv(divSrc, s_gpllFreq / rate);
            HAL_CRU_ClkSetMux(muxSrc, 0);
            HAL_CRU_ClkSetMux(mux, 0);
            HAL_CRU_ClkDisable(fracGateId);
        } else if ((!(s_vpllFreq % rate)) && ((s_vpllFreq / rate) < 31)) {
            HAL_CRU_ClkSetDiv(divSrc, s_vpllFreq / rate);
            HAL_CRU_ClkSetMux(muxSrc, 1);
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
        HAL_CRU_ClkSetMux(mux, 0);

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
    if (!s_vpllFreq) {
        s_vpllFreq = HAL_CRU_GetPllFreq(&VPLL);
    }

    switch (clockName) {
    case PLL_GPLL:
        freq = HAL_CRU_GetPllFreq(&GPLL);
        s_gpllFreq = freq;

        return freq;
    case PLL_VPLL:
        freq = HAL_CRU_GetPllFreq(&VPLL);
        s_vpllFreq = freq;

        return freq;
    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case MCLK_I2S8CH_0_TX:
    case MCLK_I2S8CH_0_RX:
    case MCLK_I2S8CH_1_TX:
    case MCLK_I2S8CH_1_RX:
    case CLK_AUDIOPWM:
    case CLK_32K:

        return HAL_CRU_ClkFracGetFreq(clockName);
    case MCLKOUT_I2S8CH_0:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = HAL_CRU_ClkGetFreq(MCLK_I2S8CH_0_TX);
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = HAL_CRU_ClkGetFreq(MCLK_I2S8CH_0_RX);
        } else {
            freq = PLL_INPUT_OSC_RATE / 2;
        }

        return freq;
    case CLK_CODEC:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            freq = HAL_CRU_ClkGetFreq(MCLK_I2S8CH_1_RX);
        } else {
            freq = HAL_CRU_ClkGetFreq(MCLK_I2S8CH_1_TX);
        }

        return freq;
    case CLK_TSADC:
    case CLK_SARADC:
    case CLK_EFUSE:
    case SCLK_M4F0:
    case SCLK_M0:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case PCLK_MCU_BUS:
        pRate = HAL_CRU_ClkGetFreq(HCLK_MCU_BUS);
        break;
    case CLK_CRYPTO:
    case CLK_CRYPTO_PKA:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            if (HAL_CRU_ClkGetMux(CLK_GET_MUX(CLK_CRYPTO_NP5))) {
                pRate = PLL_INPUT_OSC_RATE;
            }
            freq = (pRate * 2) / ((HAL_CRU_ClkGetDiv(CLK_GET_DIV(CLK_CRYPTO_NP5)) - 1) * 2 + 3);
        } else {
            if (HAL_CRU_ClkGetMux(CLK_GET_MUX(CLK_CRYPTO_D))) {
                pRate = PLL_INPUT_OSC_RATE;
            }
            freq = pRate / HAL_CRU_ClkGetDiv(CLK_GET_DIV(CLK_CRYPTO_D));
        }

        return freq;
    case CLK_GPLL_MUX_NP5:
        freq = (pRate * 2) / ((HAL_CRU_ClkGetDiv(clkDiv) - 1) * 2 + 3);

        return freq;
    case CLK_HIFI3:
        if (HAL_CRU_ClkGetMux(CLK_GET_MUX(CLK_HIFI3_SRC))) {
            pRate = s_vpllFreq;
        } else {
            pRate = s_gpllFreq;
        }
        if (HAL_CRU_ClkGetMux(clkMux)) {
            freq = pRate * 2 / ((HAL_CRU_ClkGetDiv(CLK_GET_DIV(CLK_HIFI3_NP5)) - 1) * 2 + 3);
        } else {
            freq = pRate / HAL_CRU_ClkGetDiv(CLK_GET_DIV(CLK_HIFI3_D));
        }

        return freq;
    case CLK_SDMMC:
    case CLK_TIMER:
    case CLK_PWM0:
    case CLK_PWM1:
    case CLK_PWM2:
    case CLK_SPI0:
    case CLK_SPI1:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = PLL_INPUT_OSC_RATE;
        }
        break;
    case CLK_I2C_CODEC:
    case CLK_XIP_SFC0:
    case CLK_XIP_SFC1:
    case CLK_XIP_HYPERX8:
        if (HAL_CRU_ClkGetMux(clkMux) == 3) {
            pRate = PLL_INPUT_OSC_RATE;
        }
        break;
    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_I2C2:
    case CLK_VIP_OUT:
    case HCLK_TOP_BUS:
    case PCLK_TOP_BUS:
        if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            pRate = PLL_INPUT_OSC_RATE;
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
    if (!s_vpllFreq) {
        s_vpllFreq = HAL_CRU_GetPllFreq(&VPLL);
    }

    switch (clockName) {
    case PLL_GPLL:
        error = HAL_CRU_SetPllFreq(&GPLL, rate);
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);

        return error;
    case PLL_VPLL:
        error = HAL_CRU_SetPllFreq(&VPLL, rate);
        s_vpllFreq = HAL_CRU_GetPllFreq(&VPLL);

        return error;
    case HCLK_MCU_BUS:
        HAL_SystemCoreClockUpdate(rate, HAL_SYSTICK_CLKSRC_EXT);
        break;

    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case MCLK_I2S8CH_0_TX:
    case MCLK_I2S8CH_0_RX:
    case MCLK_I2S8CH_1_TX:
    case MCLK_I2S8CH_1_RX:
    case CLK_AUDIOPWM:
    case CLK_32K:
        error = HAL_CRU_ClkFracSetFreq(clockName, rate);

        return error;
    case MCLKOUT_I2S8CH_0:
        if (rate == PLL_INPUT_OSC_RATE / 2) {
            HAL_CRU_ClkSetMux(clkMux, 2);
        } else if (rate == HAL_CRU_ClkGetFreq(MCLK_I2S8CH_0_TX)) {
            HAL_CRU_ClkSetMux(clkMux, 0);
        } else {
            HAL_CRU_ClkSetMux(clkMux, 1);
        }

        return HAL_OK;
    case CLK_CODEC:
        if (rate == HAL_CRU_ClkGetFreq(MCLK_I2S8CH_1_TX)) {
            HAL_CRU_ClkSetMux(clkMux, 0);
        } else {
            HAL_CRU_ClkSetMux(clkMux, 1);
        }

        return HAL_OK;
    case CLK_TSADC:
    case CLK_SARADC:
    case CLK_EFUSE:
    case SCLK_M4F0:
    case SCLK_M0:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case PCLK_MCU_BUS:
        pRate = HAL_CRU_ClkGetFreq(HCLK_MCU_BUS);
        break;
    case CLK_CRYPTO:
    case CLK_CRYPTO_PKA:
        if (!(PLL_INPUT_OSC_RATE % rate)) {
            HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_CRYPTO_D), 1);
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(CLK_CRYPTO_D), PLL_INPUT_OSC_RATE / rate);
            HAL_CRU_ClkSetMux(clkMux, 0);
        } else if (!(s_gpllFreq % rate)) {
            HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_CRYPTO_D), 0);
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(CLK_CRYPTO_D), s_gpllFreq / rate);
            HAL_CRU_ClkSetMux(clkMux, 0);
        } else {
            div = (HAL_DIV_ROUND_UP(pRate * 2, rate) - 3) / 2;
            HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_CRYPTO_NP5), 0);
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(CLK_CRYPTO_NP5), div + 1);
            HAL_CRU_ClkSetMux(clkMux, 1);
        }

        return HAL_OK;
    case CLK_GPLL_MUX_NP5:
        div = (HAL_DIV_ROUND_UP(pRate * 2, rate) - 3) / 2;
        HAL_CRU_ClkSetDiv(clkDiv, div + 1);

        return HAL_OK;
    case CLK_HIFI3:
        HAL_CRU_ClkEnable(CLK_HIFI3_DIV_GATE);
        HAL_CRU_ClkEnable(CLK_HIFI3_NP5_DIV_GATE);
        if (!(s_vpllFreq % rate)) {
            div = HAL_DIV_ROUND_UP(s_vpllFreq, rate);
            HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_HIFI3_SRC), 1);
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(CLK_HIFI3_D), div);
            HAL_CRU_ClkSetMux(clkMux, 0);
            HAL_CRU_ClkDisable(CLK_HIFI3_NP5_DIV_GATE);
        } else if (HAL_CRU_ClkNp5BestDiv(CLK_HIFI3_NP5, rate, s_vpllFreq, &div) == HAL_OK) {
            HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_HIFI3_SRC), 1);
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(CLK_HIFI3_NP5), div + 1);
            HAL_CRU_ClkSetMux(clkMux, 1);
            HAL_CRU_ClkDisable(CLK_HIFI3_DIV_GATE);
        } else if (HAL_CRU_ClkNp5BestDiv(CLK_HIFI3_NP5, rate, s_gpllFreq, &div) == HAL_OK) {
            HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_HIFI3_SRC), 0);
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(CLK_HIFI3_NP5), div + 1);
            HAL_CRU_ClkSetMux(clkMux, 1);
            HAL_CRU_ClkDisable(CLK_HIFI3_DIV_GATE);
        } else {
            div = HAL_DIV_ROUND_UP(s_gpllFreq, rate);
            HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_HIFI3_SRC), 0);
            HAL_CRU_ClkSetDiv(CLK_GET_DIV(CLK_HIFI3_D), div);
            HAL_CRU_ClkSetMux(clkMux, 0);
            HAL_CRU_ClkDisable(CLK_HIFI3_NP5_DIV_GATE);
        }

        return HAL_OK;
    case CLK_SDMMC:
    case CLK_TIMER:
    case CLK_PWM0:
    case CLK_PWM1:
    case CLK_PWM2:
    case CLK_SPI0:
    case CLK_SPI1:
        if (!(PLL_INPUT_OSC_RATE % rate)) {
            mux = 1;
            pRate = PLL_INPUT_OSC_RATE;
        }
        break;
    case CLK_I2C_CODEC:
    case CLK_XIP_SFC0:
    case CLK_XIP_SFC1:
    case CLK_XIP_HYPERX8:
        if (!(PLL_INPUT_OSC_RATE % rate)) {
            mux = 3;
            pRate = PLL_INPUT_OSC_RATE;
        }
        break;
    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_I2C2:
    case CLK_VIP_OUT:
    case HCLK_TOP_BUS:
    case PCLK_TOP_BUS:
        if (!(PLL_INPUT_OSC_RATE % rate)) {
            mux = 2;
            pRate = PLL_INPUT_OSC_RATE;
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
        mask = CRU_GLB_RST_CON_WDT0_GLB_SRST_EN_MASK | CRU_GLB_RST_CON_WDT0_GLB_SRST_CTRL_SHIFT;
        val = (1 << CRU_GLB_RST_CON_WDT0_GLB_SRST_EN_SHIFT) | (1 << CRU_GLB_RST_CON_WDT0_GLB_SRST_CTRL_SHIFT);
        break;
    case GLB_RST_FST_WDT1:
        mask = CRU_GLB_RST_CON_WDT1_GLB_SRST_EN_MASK | CRU_GLB_RST_CON_WDT1_GLB_SRST_CTRL_SHIFT;
        val = (1 << CRU_GLB_RST_CON_WDT1_GLB_SRST_EN_SHIFT) | (1 << CRU_GLB_RST_CON_WDT1_GLB_SRST_CTRL_SHIFT);
        break;
    case GLB_RST_FST_WDT2:
        mask = CRU_GLB_RST_CON_WDT2_GLB_SRST_EN_MASK | CRU_GLB_RST_CON_WDT2_GLB_SRST_CTRL_SHIFT;
        val = (1 << CRU_GLB_RST_CON_WDT2_GLB_SRST_EN_SHIFT) | (1 << CRU_GLB_RST_CON_WDT2_GLB_SRST_CTRL_SHIFT);
        break;
    case GLB_RST_SND_WDT0:
        mask = CRU_GLB_RST_CON_WDT0_GLB_SRST_EN_MASK | CRU_GLB_RST_CON_WDT0_GLB_SRST_CTRL_SHIFT;
        val = (1 << CRU_GLB_RST_CON_WDT0_GLB_SRST_EN_SHIFT);
        break;
    case GLB_RST_SND_WDT1:
        mask = CRU_GLB_RST_CON_WDT1_GLB_SRST_EN_MASK | CRU_GLB_RST_CON_WDT1_GLB_SRST_CTRL_SHIFT;
        val = (1 << CRU_GLB_RST_CON_WDT1_GLB_SRST_EN_SHIFT);
        break;
    case GLB_RST_SND_WDT2:
        mask = CRU_GLB_RST_CON_WDT2_GLB_SRST_EN_MASK | CRU_GLB_RST_CON_WDT2_GLB_SRST_CTRL_SHIFT;
        val = (1 << CRU_GLB_RST_CON_WDT2_GLB_SRST_EN_SHIFT);
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

#endif /* RKMCU_RK2206 && HAL_CRU_MODULE_ENABLED */
