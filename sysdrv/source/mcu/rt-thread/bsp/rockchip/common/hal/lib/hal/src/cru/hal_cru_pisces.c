/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(RKMCU_PISCES) && defined(HAL_CRU_MODULE_ENABLED)

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
#define CRU_AS_CFG_VAL2          0x1
#define CRU_AS_CNT_MSK           0xFFFF

/********************* Private Structure Definition **************************/

static uint32_t s_gpllFreq;
static uint32_t s_cpllFreq;

static struct PLL_SETUP GPLL = {
    .conOffset0 = &(CRU->GPLL_CON[0]),
    .conOffset1 = &(CRU->GPLL_CON[1]),
    .conOffset2 = &(CRU->GPLL_CON[2]),
    .modeOffset = &(CRU->CRU_MODE_CON00),
    .modeShift = 0,
    .lockShift = 10,
    .modeMask = 0x3 << 0,
    .rateTable = NULL,
};

static struct PLL_SETUP CPLL = {
    .conOffset0 = &(CRU->CPLL_CON[0]),
    .conOffset1 = &(CRU->CPLL_CON[1]),
    .conOffset2 = &(CRU->CPLL_CON[2]),
    .modeOffset = &(CRU->CRU_MODE_CON00),
    .modeShift = 2,
    .lockShift = 10,
    .modeMask = 0x3 << 2,
    .rateTable = NULL,
};

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/
#ifdef HAL_CRU_DCG_FEATURE_ENABLED
static void CRU_DcgConfig(uint8_t ch, uint8_t func, uint32_t val1, uint32_t val2)
{
    HAL_ASSERT(ch < 2);

    switch (func) {
    case 1: /* cfg en */
        CRU->DCG_CON[ch][4] = VAL_MASK_WE(CRU_DCG0_CON4_CFG_EN_MASK,
                                          val1 << CRU_DCG0_CON4_CFG_EN_SHIFT);
        break;
    case 2: /* period cnt */
        CRU->DCG_CON[ch][0] = val1;
        CRU->DCG_CON[ch][1] = val2;
        break;
    case 3: /* period cnt */
        CRU->DCG_CON[ch][2] = val1;
        CRU->DCG_CON[ch][3] = val2;
        break;
    case 4: /* step */
        CRU->DCG_CON[ch][4] = VAL_MASK_WE(CRU_DCG0_CON4_CFG_STEP1_MASK,
                                          val1 << CRU_DCG0_CON4_CFG_STEP1_SHIFT);
        CRU->DCG_CON[ch][4] = VAL_MASK_WE(CRU_DCG0_CON4_CFG_STEP2_MASK,
                                          val2 << CRU_DCG0_CON4_CFG_STEP2_SHIFT);
        break;
    case 5: /* limit */
        CRU->DCG_CON[ch][5] = VAL_MASK_WE(CRU_DCG0_CON5_CFG_LMT_MASK,
                                          val1 << CRU_DCG0_CON5_CFG_LMT_SHIFT);
        break;
    default:
        break;
    }
}
#endif

#ifdef HAL_CRU_AS_FEATURE_ENABLED
static void CRU_AsConfig(uint8_t ch, uint32_t val1, uint32_t val2)
{
    HAL_ASSERT(ch < 5);
    CRU->AS_CON[ch][1] = VAL_MASK_WE(CRU_AS0_CON1_AS_CTRL_MASK |
                                     CRU_AS0_CON1_AS_CFG_MASK |
                                     CRU_AS0_CON1_ASS_EN_MASK |
                                     CRU_AS0_CON1_AS_EN_MASK,
                                     val1 << CRU_AS0_CON1_AS_CTRL_SHIFT |
                                     val2 << CRU_AS0_CON1_AS_CFG_SHIFT);
}

static void CRU_AsCntConfig(uint8_t ch, uint32_t val1, uint32_t val2)
{
    HAL_ASSERT(ch < 5);

    if (val1 >= CRU_AS_CNT_MSK) {
        val1 = CRU->AS_CON[ch][0] & CRU_AS_CNT_MSK;
    }

    if (val2 >= CRU_AS_CNT_MSK) {
        val2 = (CRU->AS_CON[ch][0] >> 16) & CRU_AS_CNT_MSK;
    }

    CRU->AS_CON[ch][0] = val1 | (val2 << 16);
}
#endif
/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup CRU_Exported_Functions_Group5 Other Functions
 *  @{
 */

#ifdef HAL_CRU_AS_FEATURE_ENABLED
void HAL_CRU_AsEnable(uint8_t ch, uint8_t en)
{
    HAL_ASSERT(ch < 5);

    if (en) {
        CRU->AS_CON[ch][1] = VAL_MASK_WE(CRU_AS0_CON1_AS_EN_MASK |
                                         CRU_AS0_CON1_ASS_EN_MASK,
                                         CRU_AS0_CON1_AS_EN_MASK |
                                         CRU_AS0_CON1_ASS_EN_MASK);
    } else {
        CRU->AS_CON[ch][1] = VAL_MASK_WE(CRU_AS0_CON1_AS_EN_MASK |
                                         CRU_AS0_CON1_ASS_EN_MASK, 0);
    }
}

void HAL_CRU_AsInit(void)
{
    CRU_AsConfig(0, 0x77, CRU_AS_CFG_VAL2);
    CRU_AsCntConfig(0, 0x20, 0x18);

    CRU_AsConfig(1, 0x99, CRU_AS_CFG_VAL2);
    CRU_AsCntConfig(1, 0x20, 0x18);

    CRU_AsConfig(2, 0x1, CRU_AS_CFG_VAL2);
    CRU_AsCntConfig(2, 0x20, 0x18);

    CRU_AsConfig(3, 0x99, CRU_AS_CFG_VAL2);
    CRU_AsCntConfig(3, 0x20, 0x18);

    CRU_AsConfig(4, 0x99, CRU_AS_CFG_VAL2);
    CRU_AsCntConfig(4, 0x20, 0x18);

    HAL_CRU_AsEnable(1, 1);
    HAL_CRU_AsEnable(2, 1);
    HAL_CRU_AsEnable(3, 1);
    HAL_CRU_AsEnable(4, 1);
}
#endif

HAL_Status HAL_CRU_VopDclkEnable(uint32_t gateId)
{
    HAL_Status ret = HAL_OK;

    HAL_CRU_ClkEnable(gateId);

    return ret;
}

HAL_Status HAL_CRU_VopDclkDisable(uint32_t gateId)
{
    HAL_Status ret = HAL_OK;

    HAL_CRU_ClkDisable(gateId);

    return ret;
}

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
    case I2S_MCLKOUT:
        muxSrc = CLK_GET_MUX(CLK_I2S8CH_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S8CH_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S8CH_FRAC);
        mux = CLK_GET_MUX(MCLK_I2S8CH);
        break;
    case CLK_32K:
        divFrac = CLK_GET_DIV(CLK_32K);
        break;
    default:

        return HAL_INVAL;
    }

    n = (CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0xffff0000) >> 16;
    m = CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0x0000ffff;

    if (clockName == CLK_32K) {
        pRate = PLL_INPUT_OSC_RATE;
        freq = (pRate / m) * n;

        return freq;
    }

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
    uint32_t muxSrc, mux = CLK_GET_MUX(clockName), muxOut = 0;
    uint32_t divSrc, divFrac;
    uint32_t n = 0, m = 0, pRate = s_gpllFreq;
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
    case I2S_MCLKOUT:
        muxSrc = CLK_GET_MUX(CLK_I2S8CH_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S8CH_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S8CH_FRAC);
        mux = CLK_GET_MUX(MCLK_I2S8CH);
        muxOut = CLK_GET_MUX(I2S_MCLKOUT);
        gateId = CLK_I2S8CH_GATE;
        fracGateId = CLK_I2S8CH_FRAC_GATE;
        break;
    case CLK_32K:
        divFrac = CLK_GET_DIV(CLK_32K);
        break;
    default:

        return HAL_INVAL;
    }

    if (clockName == CLK_32K) {
        HAL_CRU_FracdivGetConfig(rate, PLL_INPUT_OSC_RATE, &n, &m);
        CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;

        return HAL_OK;
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
    } else {
        HAL_CRU_FracdivGetConfig(rate, pRate, &n, &m);
        HAL_CRU_ClkSetDiv(divSrc, 1);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;
        HAL_CRU_ClkSetMux(mux, 1);
    }

    if (muxOut) {
        HAL_CRU_ClkSetMux(muxOut, 0);
    }

    return HAL_OK;
}

/**
 * @brief Get DPHY clk freq.
 * @param  clockName: CLOCK_Name id
 * @return clk rate.
 * How to calculate the Frac clk divider:
 *     numerator is frac register[31:16]
 *     denominator is frac register[15:0]
 *     clk rate = pRate * numerator / denominator
 */
static uint32_t HAL_CRU_ClkDphyGetFreq(eCLOCK_Name clockName)
{
    uint32_t freq = 0;
    uint32_t muxSrc, mux = CLK_GET_MUX(clockName);
    uint32_t divFrac;
    uint32_t n, m, pRate;

    switch (clockName) {
    case CLK_DPHY_REF:
        muxSrc = CLK_GET_MUX(CLK_DPHY_REF_FRAC);
        divFrac = CLK_GET_DIV(CLK_DPHY_REF_FRAC);
        break;
    default:

        return HAL_INVAL;
    }
    n = (CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0xffff0000) >> 16;
    m = CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0x0000ffff;

    if (HAL_CRU_ClkGetMux(muxSrc)) {
        pRate = s_cpllFreq;
    } else {
        pRate = s_gpllFreq;
    }

    if (HAL_CRU_ClkGetMux(mux) == 0) {
        freq = PLL_INPUT_OSC_RATE;
    } else if (HAL_CRU_ClkGetMux(mux) == 1) {
        freq = (pRate / m) * n;
    }

    return freq;
}

/**
 * @brief Set DPHY clk freq.
 * @param  clockName: CLOCK_Name id.
 * @param  rate: clk set rate
 * @return HAL_Status.
 * How to calculate the Frac clk divider:
 *     if rate is inpust osc rate, select input osc
 *     else select ifrac divider
 */
static HAL_Status HAL_CRU_ClkDphySetFreq(eCLOCK_Name clockName, uint32_t rate)
{
    uint32_t muxSrc, mux = CLK_GET_MUX(clockName);
    uint32_t divFrac;
    uint32_t n = 0, m = 0, pRate = s_gpllFreq;

    switch (clockName) {
    case CLK_DPHY_REF:
        muxSrc = CLK_GET_MUX(CLK_DPHY_REF_FRAC);
        divFrac = CLK_GET_DIV(CLK_DPHY_REF_FRAC);
        break;
    default:

        return HAL_INVAL;
    }

    if (rate == PLL_INPUT_OSC_RATE) {
        HAL_CRU_ClkSetMux(mux, 0);
    } else {
        HAL_CRU_FracdivGetConfig(rate, pRate, &n, &m);
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
    uint32_t pRate = s_gpllFreq;

    if (!s_gpllFreq) {
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    }
    if (!s_cpllFreq) {
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);
    }

    switch (clockName) {
    case PLL_GPLL:
        freq = HAL_CRU_GetPllFreq(&GPLL);
        s_gpllFreq = freq;

        return freq;
    case PLL_CPLL:
        freq = HAL_CRU_GetPllFreq(&CPLL);
        s_cpllFreq = freq;

        return freq;
    case DCLK_VOP_S:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = s_cpllFreq;
        }

        freq = pRate / HAL_CRU_ClkGetDiv(clkDiv);

        return freq;
    case CLK_UART0:
    case CLK_UART1:
    case I2S_MCLKOUT:
    case CLK_32K:
        freq = HAL_CRU_ClkFracGetFreq(clockName);

        return freq;
    case CLK_GPIO_DBG0:
    case CLK_GPIO_DBG1:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case CLK_DPHY_REF:
        freq = HAL_CRU_ClkDphyGetFreq(clockName);

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
    uint32_t clkMux = CLK_GET_MUX(clockName), mux = 0;
    uint32_t clkDiv = CLK_GET_DIV(clockName), div = 0;
    uint32_t pRate = s_gpllFreq;

    if (!s_gpllFreq) {
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    }
    if (!s_cpllFreq) {
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);
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
    case DCLK_VOP_S:
        if (s_gpllFreq % rate) {
            div = HAL_DIV_ROUND_UP(DCLK_LCDC_PLL_LIMIT_FREQ, rate);
            error = HAL_CRU_SetPllFreq(&CPLL, rate * div);
            s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);
            HAL_CRU_ClkSetMux(clkMux, 1);
            HAL_CRU_ClkSetDiv(clkDiv, div);
        } else {
            div = HAL_DIV_ROUND_UP(s_gpllFreq, rate);
            HAL_CRU_ClkSetMux(clkMux, 0);
            HAL_CRU_ClkSetDiv(clkDiv, div);
        }

        return error;
    case CLK_UART0:
    case CLK_UART1:
    case I2S_MCLKOUT:
    case CLK_32K:
        error = HAL_CRU_ClkFracSetFreq(clockName, rate);

        return error;
    case CLK_GPIO_DBG0:
    case CLK_GPIO_DBG1:
        pRate = PLL_INPUT_OSC_RATE;
        break;
    case CLK_DPHY_REF:
        error = HAL_CRU_ClkDphySetFreq(clockName, rate);

        return error;
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

    if (clockName == HCLK_M4) {
        HAL_SystemCoreClockUpdate(rate, HAL_SYSTICK_CLKSRC_EXT);
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
        mask = CRU_GLB_RST_CON_WDT0_GLBRST_EN_MASK | CRU_GLB_RST_CON_WDT0_FST_GLBRST_EN_MASK;
        val = (1 << CRU_GLB_RST_CON_WDT0_GLBRST_EN_SHIFT) | (1 << CRU_GLB_RST_CON_WDT0_FST_GLBRST_EN_SHIFT);
        break;
    case GLB_RST_FST_WDT1:
        mask = CRU_GLB_RST_CON_WDT1_GLBRST_EN_MASK | CRU_GLB_RST_CON_WDT1_FST_GLBRST_EN_MASK;
        val = (1 << CRU_GLB_RST_CON_WDT1_GLBRST_EN_SHIFT) | (1 << CRU_GLB_RST_CON_WDT1_FST_GLBRST_EN_SHIFT);
        break;
    case GLB_RST_SND_WDT0:
        mask = CRU_GLB_RST_CON_WDT0_GLBRST_EN_MASK | CRU_GLB_RST_CON_WDT0_FST_GLBRST_EN_MASK;
        val = (1 << CRU_GLB_RST_CON_WDT0_GLBRST_EN_SHIFT);
        break;
    case GLB_RST_SND_WDT1:
        mask = CRU_GLB_RST_CON_WDT1_GLBRST_EN_MASK | CRU_GLB_RST_CON_WDT1_FST_GLBRST_EN_MASK;
        val = (1 << CRU_GLB_RST_CON_WDT1_GLBRST_EN_SHIFT);
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

#endif /* RKMCU_PISCES && HAL_CRU_MODULE_ENABLED */
