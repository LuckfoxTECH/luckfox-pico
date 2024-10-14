/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(RKMCU_RK2106) && defined(HAL_CRU_MODULE_ENABLED)

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
#define USB480M_FREQ 480000000
/********************* Private Structure Definition **************************/

static struct PLL_CONFIG PLL_TABLE[] = {
    /* _mhz, _refDiv, _fbDiv, _postdDv1, _postDiv2, _dsmpd, _frac */
    RK_PLL_RATE(600000000, 3, 75, 1, 1, 1, 0),
    RK_PLL_RATE(400000000, 1, 50, 3, 1, 1, 0),
    RK_PLL_RATE(300000000, 2, 100, 4, 1, 1, 0),
    { 0 /* sentinel */ },
};

static uint32_t s_apllFreq;
static uint32_t s_acodecpllFreq = 0;

static struct PLL_SETUP APLL = {
    .conOffset0 = &(CRU->APLL_CON[0]),
    .conOffset1 = &(CRU->APLL_CON[1]),
    .conOffset2 = &(CRU->APLL_CON[2]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 0,
    .lockShift = 10,
    .modeMask = 0x1 << 0,
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
    uint32_t divSrc = 0, divFrac = 0;
    uint32_t n, m, pRate;

    switch (clockName) {
    case CLK_I2S0:
        muxSrc = CLK_GET_MUX(CLK_I2S0_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S0_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S0_FRAC);
        break;
    case CLK_I2S1:
        muxSrc = CLK_GET_MUX(CLK_I2S1_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S1_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S1_FRAC);
        break;
    default:

        return 0;
    }

    n = (CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0xffff0000) >> 16;
    m = CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0x0000ffff;

    if (HAL_CRU_ClkGetMux(muxSrc)) {
        pRate = s_acodecpllFreq / HAL_CRU_ClkGetDiv(divSrc);
    } else {
        pRate = s_apllFreq / HAL_CRU_ClkGetDiv(divSrc);
    }

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
    case CLK_I2S0:
        muxSrc = CLK_GET_MUX(CLK_I2S0_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S0_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S0_FRAC);
        gateId = CLK_I2S0_SRC_GATE;
        fracGateId = CLK_I2S0_FRAC_SRC_GATE;
        break;
    case CLK_I2S1:
        muxSrc = CLK_GET_MUX(CLK_I2S1_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S1_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S1_FRAC);
        gateId = CLK_I2S1_SRC_GATE;
        fracGateId = CLK_I2S1_FRAC_SRC_GATE;
        break;
    default:

        return HAL_INVAL;
    }
    HAL_CRU_ClkEnable(gateId);
    HAL_CRU_ClkEnable(fracGateId);
    if (PLL_INPUT_OSC_RATE / 2 == rate) {
        HAL_CRU_ClkSetMux(mux, 2);
        HAL_CRU_ClkDisable(gateId);
    } else if ((!(s_apllFreq % rate)) && ((s_apllFreq / rate) < 63)) {
        HAL_CRU_ClkSetDiv(divSrc, s_apllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        HAL_CRU_ClkSetMux(mux, 0);
        HAL_CRU_ClkDisable(fracGateId);
    } else if ((!(s_acodecpllFreq % rate)) && ((s_acodecpllFreq / rate) < 63)) {
        HAL_CRU_ClkSetDiv(divSrc, s_acodecpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 1);
        HAL_CRU_ClkSetMux(mux, 0);
        HAL_CRU_ClkDisable(fracGateId);
    } else {
        HAL_CRU_FracdivGetConfig(rate, s_apllFreq, &n, &m);
        HAL_CRU_ClkSetDiv(divSrc, 1);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;
        HAL_CRU_ClkSetMux(mux, 1);
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
    uint32_t pRate = s_apllFreq;

    if (!s_apllFreq) {
        s_apllFreq = HAL_CRU_GetPllFreq(&APLL);
    }

    switch (clockName) {
    case PLL_APLL:
        freq = HAL_CRU_GetPllFreq(&APLL);
        s_apllFreq = freq;

        return freq;
    case CAL_CORE:
    case SYS_CORE:
    case DCLK_EBC:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = USB480M_FREQ;
        } else {
            pRate = s_apllFreq;
        }
        break;
    case CAL_STCLK_CORE:
        pRate = HAL_CRU_ClkGetFreq(CAL_CORE);
        break;
    case SYS_STCLK_CORE:
    case PCLK_LOGIC:
        pRate = HAL_CRU_ClkGetFreq(SYS_CORE);
        break;
    case CLK_UART0:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
    case CLK_SDMMC:
    case CLK_EMMC:
    case CLK_SPI0:
    case CLK_SPI1:
    case CLK_SFC:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = PLL_INPUT_OSC_RATE;
        } else {
            pRate = s_apllFreq;
        }
        break;
    case CLK_UART1:
        if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            pRate = PLL_INPUT_OSC_RATE;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            pRate = USB480M_FREQ;
        } else {
            pRate = s_apllFreq;
        }
        break;
    case CLK_I2S0:
    case CLK_I2S1:
        freq = HAL_CRU_ClkFracGetFreq(clockName);

        return freq;
    case CLK_I2S1_OUT:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            freq = PLL_INPUT_OSC_RATE / 2;
        } else {
            freq = HAL_CRU_ClkFracGetFreq(CLK_I2S1);
        }

        return freq;
    case CLK_SARADC:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case CLK_TIMER0:
    case CLK_TIMER1:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = HAL_CRU_ClkGetFreq(PCLK_LOGIC);
        } else {
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
    uint32_t pRate = s_apllFreq;

    if (!s_apllFreq) {
        s_apllFreq = HAL_CRU_GetPllFreq(&APLL);
    }

    switch (clockName) {
    case PLL_APLL:
        error = HAL_CRU_SetPllFreq(&APLL, rate);
        s_apllFreq = HAL_CRU_GetPllFreq(&APLL);

        return error;
    case CAL_CORE:
    case SYS_CORE:
    case DCLK_EBC:
        if ((USB480M_FREQ % rate) == 0) {
            pRate = USB480M_FREQ;
            mux = 1;
        } else {
            pRate = s_apllFreq;
            mux = 0;
        }
        break;
    case CAL_STCLK_CORE:
        pRate = HAL_CRU_ClkGetFreq(CAL_CORE);
        break;
    case SYS_STCLK_CORE:
    case PCLK_LOGIC:
        pRate = HAL_CRU_ClkGetFreq(SYS_CORE);
        break;
    case CLK_UART0:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
    case CLK_SDMMC:
    case CLK_EMMC:
    case CLK_SPI0:
    case CLK_SPI1:
    case CLK_SFC:
        if ((PLL_INPUT_OSC_RATE % rate) == 0) {
            pRate = PLL_INPUT_OSC_RATE;
            mux = 1;
        } else {
            pRate = s_apllFreq;
            mux = 0;
        }
        break;
    case CLK_UART1:
        if ((PLL_INPUT_OSC_RATE % rate) == 0) {
            pRate = PLL_INPUT_OSC_RATE;
            mux = 1;
        } else if ((USB480M_FREQ % rate) == 0) {
            pRate = USB480M_FREQ;
            mux = 2;
        } else {
            pRate = s_apllFreq;
            mux = 0;
        }
        break;
    case CLK_I2S0:
    case CLK_I2S1:
        error = HAL_CRU_ClkFracSetFreq(clockName, rate);

        return error;
    case CLK_I2S1_OUT:
        if (rate == PLL_INPUT_OSC_RATE / 2) {
            HAL_CRU_ClkSetMux(clkMux, 1);
        } else {
            error = HAL_CRU_ClkFracSetFreq(CLK_I2S1, rate);
            HAL_CRU_ClkSetMux(clkMux, 0);
        }

        return HAL_OK;
    case CLK_SARADC:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case CLK_TIMER0:
    case CLK_TIMER1:
        if ((PLL_INPUT_OSC_RATE % rate) == 0) {
            pRate = PLL_INPUT_OSC_RATE;
            mux = 0;
        } else {
            pRate = HAL_CRU_ClkGetFreq(PCLK_LOGIC);
            mux = 1;
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

/** @} */

/** @} */

/** @} */

#endif /* RKMCU_RK2106 && HAL_CRU_MODULE_ENABLED */
