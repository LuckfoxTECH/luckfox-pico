/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(SOC_RV1106) && defined(HAL_CRU_MODULE_ENABLED)

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

#define OSC_32KHZ        32768
#define PCLK_DBG_RATE    (150 * MHZ)
#define CLK_PVTPLL0_RATE 0
#define CLK_PVTPLL1_RATE 0

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
static uint32_t s_gpllFreq;

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
    case CLK_UART5:
        muxSrc = CLK_GET_MUX(CLK_UART5_SRC);
        divSrc = CLK_GET_DIV(CLK_UART5_SRC);
        divFrac = CLK_GET_DIV(CLK_UART5_FRAC);
        break;
    case CLK_REF_MIPI0:
        muxSrc = CLK_GET_MUX(CLK_REF_MIPI0_SRC);
        divSrc = CLK_GET_DIV(CLK_REF_MIPI0_SRC);
        divFrac = CLK_GET_DIV(CLK_REF_MIPI0_FRAC);
        break;
    case CLK_REF_MIPI1:
        muxSrc = CLK_GET_MUX(CLK_REF_MIPI1_SRC);
        divSrc = CLK_GET_DIV(CLK_REF_MIPI1_SRC);
        divFrac = CLK_GET_DIV(CLK_REF_MIPI1_FRAC);
        break;
    case CLK_VICAP_M0:
        muxSrc = CLK_GET_MUX(CLK_VICAP_M0_SRC);
        divSrc = CLK_GET_DIV(CLK_VICAP_M0_SRC);
        divFrac = CLK_GET_DIV(CLK_VICAP_M0_FRAC);
        break;
    case CLK_VICAP_M1:
        muxSrc = CLK_GET_MUX(CLK_VICAP_M1_SRC);
        divSrc = CLK_GET_DIV(CLK_VICAP_M1_SRC);
        divFrac = CLK_GET_DIV(CLK_VICAP_M1_FRAC);
        break;
    default:

        return HAL_INVAL;
    }

    HAL_CRU_ClkGetFracDiv(divFrac, &n, &m);

    if (HAL_CRU_ClkGetMux(muxSrc)) {
        pRate = s_cpllFreq / HAL_CRU_ClkGetDiv(divSrc);
    } else {
        pRate = s_gpllFreq / HAL_CRU_ClkGetDiv(divSrc);
    }

    if (HAL_CRU_ClkGetMux(mux) == 0) {
        freq = pRate;
    } else if (HAL_CRU_ClkGetMux(mux) == 1) {
        freq = (pRate / m) * n;
    } else if (HAL_CRU_ClkGetMux(mux) == 2) {
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
        muxSrc = CLK_GET_MUX(CLK_UART0_SRC);
        divSrc = CLK_GET_DIV(CLK_UART0_SRC);
        divFrac = CLK_GET_DIV(CLK_UART0_FRAC);
        gateId = CLK_UART0_GATE;
        fracGateId = CLK_UART0_FRAC_GATE;
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
    case CLK_REF_MIPI0:
        muxSrc = CLK_GET_MUX(CLK_REF_MIPI0_SRC);
        divSrc = CLK_GET_DIV(CLK_REF_MIPI0_SRC);
        divFrac = CLK_GET_DIV(CLK_REF_MIPI0_FRAC);
        gateId = CLK_REF_MIPI0_SRC_GATE;
        fracGateId = CLK_REF_MIPI0_FRAC_GATE;
        break;
    case CLK_REF_MIPI1:
        muxSrc = CLK_GET_MUX(CLK_REF_MIPI1_SRC);
        divSrc = CLK_GET_DIV(CLK_REF_MIPI1_SRC);
        divFrac = CLK_GET_DIV(CLK_REF_MIPI1_FRAC);
        gateId = CLK_REF_MIPI1_SRC_GATE;
        fracGateId = CLK_REF_MIPI1_FRAC_GATE;
        break;
    case CLK_VICAP_M0:
        muxSrc = CLK_GET_MUX(CLK_VICAP_M0_SRC);
        divSrc = CLK_GET_DIV(CLK_VICAP_M0_SRC);
        divFrac = CLK_GET_DIV(CLK_VICAP_M0_FRAC);
        gateId = CLK_VICAP_M0_SRC_GATE;
        fracGateId = CLK_VICAP_M0_FRAC_GATE;
        break;
    case CLK_VICAP_M1:
        muxSrc = CLK_GET_MUX(CLK_VICAP_M1_SRC);
        divSrc = CLK_GET_DIV(CLK_VICAP_M1_SRC);
        divFrac = CLK_GET_DIV(CLK_VICAP_M1_FRAC);
        gateId = CLK_VICAP_M1_SRC_GATE;
        fracGateId = CLK_VICAP_M1_FRAC_GATE;
        break;
    default:

        return HAL_INVAL;
    }

    HAL_CRU_ClkEnable(gateId);
    HAL_CRU_ClkEnable(fracGateId);

    if (PLL_INPUT_OSC_RATE == rate) {
        HAL_CRU_ClkSetMux(mux, 2);
        HAL_CRU_ClkDisable(gateId);
    } else if ((!(s_gpllFreq % rate)) && ((s_gpllFreq / rate) < 31)) {
        HAL_CRU_ClkSetDiv(divSrc, s_gpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        HAL_CRU_ClkSetMux(mux, 0);
        HAL_CRU_ClkDisable(fracGateId);
    } else if ((!(s_cpllFreq % rate)) && ((s_cpllFreq / rate) < 31)) {
        HAL_CRU_ClkSetDiv(divSrc, s_cpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 1);
        HAL_CRU_ClkSetMux(mux, 0);
        HAL_CRU_ClkDisable(fracGateId);
    } else {
        HAL_CRU_FracdivGetConfig(rate, s_gpllFreq, &n, &m);
        HAL_CRU_ClkSetDiv(divSrc, 1);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        HAL_CRU_ClkSetFracDiv(divFrac, n, m);
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
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t pRate = 0, freq;

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
    case PLL_GPLL:
        freq = HAL_CRU_GetPllFreq(&GPLL);
        s_gpllFreq = freq;

        return freq;
    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
    case CLK_I2S0_8CH_TX:
    case CLK_I2S0_8CH_RX:
    case CLK_REF_MIPI0:
    case CLK_REF_MIPI1:
    case CLK_VICAP_M0:
    case CLK_VICAP_M1:
        freq = HAL_CRU_ClkFracGetFreq(clockName);

        return freq;
    case DCLK_VOP:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = s_cpllFreq;
        } else {
            pRate = s_gpllFreq;
        }
        break;
    case PCLK_TOP_ROOT:
    case PCLK_NPU_ROOT:
    case PCLK_PERI_ROOT:
    case CLK_PWM0:
    case CLK_PWM1:
    case CLK_PWM2:
    case PCLK_VEPU_ROOT:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 100000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 50000000;
        } else {
            freq = PLL_INPUT_OSC_RATE;
        }

        return freq;

    case HCLK_NPU_ROOT:
    case HCLK_VI_ROOT:
    case PCLK_VI_ROOT:
    case PCLK_VO_ROOT:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 150000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 100000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = 50000000;
        } else {
            freq = PLL_INPUT_OSC_RATE;
        }

        return freq;
    case CLK_CORE_VEPU:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 400000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 300000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = CLK_PVTPLL0_RATE;
        } else {
            freq = CLK_PVTPLL1_RATE;
        }

        return freq;
    case ACLK_NPU_ROOT:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 500000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 300000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = CLK_PVTPLL0_RATE;
        } else {
            freq = CLK_PVTPLL1_RATE;
        }

        return freq;
    case ACLK_BUS_ROOT:
    case ACLK_VEPU_ROOT:
    case ACLK_VI_ROOT:
    case DCLK_VICAP:
    case ACLK_VOP_ROOT:
    case ACLK_MAC_ROOT:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 300000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 200000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = 100000000;
        } else {
            freq = PLL_INPUT_OSC_RATE;
        }

        return freq;
    case ACLK_PERI_ROOT:
    case ACLK_VEPU_COM_ROOT:
    case ACLK_VO_ROOT:
    case CLK_CORE_RGA2E:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 400000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 200000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = 100000000;
        } else {
            freq = PLL_INPUT_OSC_RATE;
        }

        return freq;

    case HCLK_PERI_ROOT:
    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_I2C2:
    case CLK_I2C3:
    case CLK_I2C4:
    case CLK_SPI1:
    case CLK_SPI0:
    case HCLK_VEPU_ROOT:
    case HCLK_VO_ROOT:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 200000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 100000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = 50000000;
        } else {
            freq = PLL_INPUT_OSC_RATE;
        }

        return freq;
    case CLK_EMMC:
    case CLK_SDMMC:
    case CLK_SDIO:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = PLL_INPUT_OSC_RATE;
        } else {
            pRate = 400000000;
        }
        break;
    case CLK_SFC:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            pRate = 500000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            pRate = 300000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            pRate = 200000000;
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
    case PLL_GPLL:
        error = HAL_CRU_SetPllFreq(&GPLL, rate);
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);

        return error;
    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
    case CLK_I2S0_8CH_TX:
    case CLK_I2S0_8CH_RX:
    case CLK_REF_MIPI0:
    case CLK_REF_MIPI1:
    case CLK_VICAP_M0:
    case CLK_VICAP_M1:
        error = HAL_CRU_ClkFracSetFreq(clockName, rate);

        return error;
    case DCLK_VOP:
        if (s_cpllFreq % rate == 0) {
            pRate = s_cpllFreq;
            mux = 1;
        } else {
            pRate = s_gpllFreq;
            mux = 0;
        }

        break;
    case PCLK_TOP_ROOT:
    case PCLK_NPU_ROOT:
    case PCLK_PERI_ROOT:
    case CLK_PWM0:
    case CLK_PWM1:
    case CLK_PWM2:
    case PCLK_VEPU_ROOT:
        if (rate >= 99000000) {
            mux = 0;
        } else if (rate >= 49000000) {
            mux = 1;
        } else {
            mux = 2;
        }

        break;

    case HCLK_NPU_ROOT:
    case HCLK_VI_ROOT:
    case PCLK_VI_ROOT:
    case PCLK_VO_ROOT:
        if (rate >= 148000000) {
            mux = 0;
        } else if (rate >= 99000000) {
            mux = 1;
        } else if (rate >= 49000000) {
            mux = 2;
        } else {
            mux = 3;
        }

        break;
    case CLK_CORE_VEPU:
        if (rate >= 398000000) {
            mux = 0;
        } else if (rate >= 297000000) {
            mux = 1;
        } else {
            mux = 3;
        }

        break;
    case ACLK_NPU_ROOT:
        if (rate >= 500000000) {
            mux = 0;
        } else if (rate >= 297000000) {
            mux = 1;
        } else {
            mux = 3;
        }

        break;
    case ACLK_BUS_ROOT:
    case ACLK_VEPU_ROOT:
    case ACLK_VI_ROOT:
    case DCLK_VICAP:
    case ACLK_VOP_ROOT:
    case ACLK_MAC_ROOT:
        if (rate >= 297000000) {
            mux = 0;
        } else if (rate >= 198000000) {
            mux = 1;
        } else if (rate >= 99000000) {
            mux = 2;
        } else {
            mux = 3;
        }

        break;
    case ACLK_PERI_ROOT:
    case ACLK_VEPU_COM_ROOT:
    case ACLK_VO_ROOT:
    case CLK_CORE_RGA2E:
        if (rate >= 396000000) {
            mux = 0;
        } else if (rate >= 198000000) {
            mux = 1;
        } else if (rate >= 99000000) {
            mux = 2;
        } else {
            mux = 3;
        }

        break;
    case HCLK_PERI_ROOT:
    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_I2C2:
    case CLK_I2C3:
    case CLK_I2C4:
    case CLK_SPI1:
    case CLK_SPI0:
    case HCLK_VEPU_ROOT:
    case HCLK_VO_ROOT:
        if (rate >= 198000000) {
            mux = 0;
        } else if (rate >= 99000000) {
            mux = 1;
        } else if (rate >= 49000000) {
            mux = 2;
        } else {
            mux = 3;
        }

        break;

    case CLK_EMMC:
    case CLK_SDMMC:
    case CLK_SDIO:
        if (PLL_INPUT_OSC_RATE % rate == 0) {
            pRate = PLL_INPUT_OSC_RATE;
            mux = 1;
        } else {
            pRate = 400000000;
            mux = 0;
        }

        break;

    case CLK_SFC:
        if (PLL_INPUT_OSC_RATE % rate == 0) {
            pRate = PLL_INPUT_OSC_RATE;
            mux = 3;
        } else if (200000000 % rate == 0) {
            pRate = 200000000;
            mux = 2;
        } else if (300000000 % rate == 0) {
            pRate = 300000000;
            mux = 1;
        } else {
            pRate = 500000000;
            mux = 0;
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
    uint32_t mask = CRU_GLB_RST_CON_CRU_WDT0_EN_MASK | CRU_GLB_RST_CON_CRU_WDT0_CON_MASK;
    uint32_t val = 1 << CRU_GLB_RST_CON_CRU_WDT0_EN_SHIFT;

    switch (wdtType) {
    case GLB_RST_FST_WDT0:
        val |= (1 << CRU_GLB_RST_CON_CRU_WDT0_CON_SHIFT);
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

#endif /* SOC_RV1106 && HAL_CRU_MODULE_ENABLED */
